// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDNewGameWidget.h"
#include "SlateOptMacros.h"

#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"
#include "Data/SDDataHandle.h"
#include "Widgets/Input/SEditableTextBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDNewGameWidget::Construct(const FArguments& InArgs)
{
	// ͨ��SDStyle��ȡSDMenuWidgetStyle�ڱ༭���е�MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	ChildSlot
	[
		// ����Ϸ����
		SNew(SBox)
			.WidthOverride(500.f)
			.HeightOverride(100.f)
			[
				SNew(SOverlay)
					// ����ͼƬ
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Bottom)
					[
						SNew(SImage)
							.Image(&MenuStyle->MenuItemBrush)
					]
					// ����
					+ SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					.Padding(FMargin(20.f, 0.f, 0.f, 0.f))
					[
						SNew(STextBlock)
							.Font(MenuStyle->Font_40)
							.Text(NSLOCTEXT("SDMenu", "NewGame", "NewGame"))
					]
					// �����
					+ SOverlay::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					.Padding(FMargin(0.f, 0.f, 20.f, 0.f))
					[
						SNew(SBox)
							.WidthOverride(300.f)
							.HeightOverride(60.f)
							[
								SAssignNew(EditTextBox, SEditableTextBox)
									.HintText(NSLOCTEXT("SDMenu", "RecordNameHint", "Input Record Name!"))	//�����������ʱ��ʾ
									.Font(MenuStyle->Font_30)
							]
					]
			]
	];
	
}
bool SSDNewGameWidget::AllowEnterGame()
{
	// ��ȡ������´浵��
	FText InputText = EditTextBox->GetText();

	// �����Ƿ�Ϊ��
	if (InputText.ToString().IsEmpty()) {
		return false;
	}
	// ��ѯ�Ƿ�Ϊ�����浵
	else {
		for (TArray<FString>::TIterator It(SDDataHandle::Get()->RecordDataList); It; ++It) {
			if ((*It).Equals(InputText.ToString())) {
				// ����ظ����������Ϊ��
				EditTextBox->SetText(FText::FromString(""));
				// ����ʾ��Ϊ�浵���ظ�
				EditTextBox->SetHintText(NSLOCTEXT("SDMenu", "RecordNameRepeateHint", "Record Name Repeated!"));
				// @TODO������������ʾ�ظ�

				return false;
			}
		}
	}

	// �����µĴ浵��
	SDDataHandle::Get()->RecordName = InputText.ToString();

	return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
