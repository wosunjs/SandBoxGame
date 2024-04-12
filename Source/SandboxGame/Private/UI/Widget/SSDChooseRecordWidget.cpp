// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDChooseRecordWidget.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"
#include "Data/SDDataHandle.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/STextComboBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDChooseRecordWidget::Construct(const FArguments& InArgs)
{
	// ͨ��SDStyle��ȡSDMenuWidgetStyle�ڱ༭���е�MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// ͨ��DataHandle�ṩ�����ݳ�ʼ�������˵��ַ���ָ������
	for (TArray<FString>::TIterator It(SDDataHandle::Get()->RecordDataList); It; ++It) {
		OptionsSource.Add(MakeShareable(new FString(*It)));
	}

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
						// �����˵�
						+ SOverlay::Slot()
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						.Padding(FMargin(0.f, 0.f, 20.f, 0.f))
						[
							SNew(SBox)
								.WidthOverride(300.f)
								.HeightOverride(60.f)
								[
									SAssignNew(RecordComboBox, STextComboBox)
										.Font(MenuStyle->Font_30)
										.OptionsSource(&OptionsSource)
								]
						]
				]
		];

	// ����Ĭ��ѡ�е�һ���浵
	RecordComboBox->SetSelectedItem(OptionsSource[0]);
}
void SSDChooseRecordWidget::UpdateRecordName()
{
	// �޸Ĵ浵��
	SDDataHandle::Get()->RecordName = *RecordComboBox->GetSelectedItem().Get();

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
