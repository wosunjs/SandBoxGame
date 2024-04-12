// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDMenuHUDWidget.h"
#include "UI/Widget/SSDMenuWidget.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SDPIScaler.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDMenuHUDWidget::Construct(const FArguments& InArgs)
{
	// 通过SDStyle获取SDMenuWidgetStyle在编辑器中的MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// 绑定控件缩放规则方法
	UIScaler.Bind(this, &SSDMenuHUDWidget::GetUIScaler);
	
	ChildSlot
	[
		SNew(SDPIScaler)		// DPI窗口大小修改时设置UIScaler
			.DPIScale(UIScaler)
			[
			SNew(SOverlay) 
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)	// 样式Horizontal水平全填充
				.VAlign(VAlign_Fill)	// 样式Vertical垂直全填充
					[
						// 添加背景图片:先实例化一个组件，然后给组件添加一个笔刷
						SNew(SImage).Image(&MenuStyle->MenuHUDBackgroundBrush)
					]
			
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)	// 样式水平居中
				.VAlign(VAlign_Center)	// 样式垂直居中
				.Expose(ImageSlot)
					[
						// 实例化SSDMenuWidget并获取其指针MenuWidget
						SAssignNew(MenuWidget, SSDMenuWidget)
					]

			
			]
	];
	
}

float SSDMenuHUDWidget::GetUIScaler() const
{
	// 窗口宽度大于1080时，定值为1，小于1080时线性变化
	auto VSW = GetViewportSize().Y;
	if (VSW < 1080.0f) {
		return VSW / 1080.0f;
	}
	return 1.0f;
}

FVector2D SSDMenuHUDWidget::GetViewportSize() const
{
	FVector2D Res(1920, 1080);
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(Res);
	}
	return Res;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
