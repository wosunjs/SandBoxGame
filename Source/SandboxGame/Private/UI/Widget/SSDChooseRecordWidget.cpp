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
	// 通过SDStyle获取SDMenuWidgetStyle在编辑器中的MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// 通过DataHandle提供的数据初始化下拉菜单字符串指针数组
	for (TArray<FString>::TIterator It(SDDataHandle::Get()->RecordDataList); It; ++It) {
		OptionsSource.Add(MakeShareable(new FString(*It)));
	}

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
						// 下拉菜单
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

	// 设置默认选中第一个存档
	RecordComboBox->SetSelectedItem(OptionsSource[0]);
}
void SSDChooseRecordWidget::UpdateRecordName()
{
	// 修改存档名
	SDDataHandle::Get()->RecordName = *RecordComboBox->GetSelectedItem().Get();

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
