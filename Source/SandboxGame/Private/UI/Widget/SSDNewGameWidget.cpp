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
	// 通过SDStyle获取SDMenuWidgetStyle在编辑器中的MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	ChildSlot
	[
		// 新游戏界面
		SNew(SBox)
			.WidthOverride(500.f)
			.HeightOverride(100.f)
			[
				SNew(SOverlay)
					// 背景图片
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Bottom)
					[
						SNew(SImage)
							.Image(&MenuStyle->MenuItemBrush)
					]
					// 文字
					+ SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					.Padding(FMargin(20.f, 0.f, 0.f, 0.f))
					[
						SNew(STextBlock)
							.Font(MenuStyle->Font_40)
							.Text(NSLOCTEXT("SDMenu", "NewGame", "NewGame"))
					]
					// 输入框
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
									.HintText(NSLOCTEXT("SDMenu", "RecordNameHint", "Input Record Name!"))	//输入框无内容时显示
									.Font(MenuStyle->Font_30)
							]
					]
			]
	];
	
}
bool SSDNewGameWidget::AllowEnterGame()
{
	// 获取输入的新存档名
	FText InputText = EditTextBox->GetText();

	// 文字是否为空
	if (InputText.ToString().IsEmpty()) {
		return false;
	}
	// 查询是否为重名存档
	else {
		for (TArray<FString>::TIterator It(SDDataHandle::Get()->RecordDataList); It; ++It) {
			if ((*It).Equals(InputText.ToString())) {
				// 如果重复设置输入框为空
				EditTextBox->SetText(FText::FromString(""));
				// 将提示改为存档名重复
				EditTextBox->SetHintText(NSLOCTEXT("SDMenu", "RecordNameRepeateHint", "Record Name Repeated!"));
				// @TODO：弹出窗口提示重复

				return false;
			}
		}
	}

	// 保存新的存档名
	SDDataHandle::Get()->RecordName = InputText.ToString();

	return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
