// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDShortcutWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Data/SDDataHandle.h"



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDShortcutWidget::Construct(const FArguments& InArgs)
{
	// 获取GameStyle的蓝图样式
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(900.f)
			.HeightOverride(160.f)
			[
				SNew(SOverlay)
					// 物品信息文字
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					[
						SAssignNew(shortcutInfoTextBlock, STextBlock)
							.Font(GameStyle->Font_Outline_40)
							.ColorAndOpacity(GameStyle->FontColor_White)
					]
					// 物品栏
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.f, 60.f, 0.f, 0.f))
					[
						SAssignNew(GridPanel, SUniformGridPanel)

					]
			]
	];

	// 设置容器未初始化
	IsInitContainer = false;

	
}

void SSDShortcutWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!IsInitContainer) {
		InitContainer();
		IsInitContainer = true;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SSDShortcutWidget::InitContainer()
{
	TArray<TSharedPtr<ShortcutContainer>> ContainerList;	// 由单个容器组成的列表

	// 容器列表初始化
	for (int i = 0; i < 9; i++) {
		// 创建容器
		TSharedPtr<SBorder> ContainerBorder;

		// 构建UI框架
		TSharedPtr<SBorder> ObjectImage;		// 物品图片
		TSharedPtr<STextBlock> ObjectNumText;	// 物品数量
		SAssignNew(ContainerBorder, SBorder)
			.Padding(FMargin(10.f))
			[
				SAssignNew(ObjectImage, SBorder)
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(0.f, 0.f, 5.f, 0.f))
					[
						SAssignNew(ObjectNumText, STextBlock)
							.Font(GameStyle->Font_Outline_20)
							.ColorAndOpacity(GameStyle->FontColor_Black)
					]
			];

		// 每初始完一个组件ContainerBorder就将其添加到GridPanel
		GridPanel->AddSlot(i, 0)
		[
			ContainerBorder->AsShared()
		] ;

		// 给快捷栏单个物品框赋值（实例化一个容器结构体）
		TSharedPtr<ShortcutContainer> Container = MakeShareable(new ShortcutContainer(ContainerBorder
			,ObjectImage , ObjectNumText, &GameStyle->NormalContainerBrush, &GameStyle->ChoosedContainerBrush, 
			&SDDataHandle::Get()->ObjectBrushList));

		// 快捷栏第一个为选中
		if (i == 0) Container->SetChoosed(true);

		// 添加到数组
		ContainerList.Add(Container);
	}

	// 将实例化的结构体注册进PlayerState的容器数组
	RegisterShortcutContainer.ExecuteIfBound(&ContainerList, shortcutInfoTextBlock);
	

}

