// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDMenuIteamWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDMenuIteamWidget::Construct(const FArguments& InArgs)
{
	// 通过SDStyle获取SDMenuWidgetStyle在编辑器中的MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	Onclicked = InArgs._Onclicked;	// 获取初始化后的OnClick对应函数（在SSDMenuWidget中完成赋值）

	ItemType = InArgs._ItemType.Get();	// 获取到的变量需要使用.Get()方法

	
	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(500.0f)
			.HeightOverride(100.0f)
			[	
				// 背景图片
				SNew(SOverlay)
					+ SOverlay::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					[
						SNew(SImage)
							.Image(&MenuStyle->MenuItemBrush)
							// 将背景图片的颜色绑定到GetTintColor函数，当函数触发时改变颜色
							.ColorAndOpacity(this, &SSDMenuIteamWidget::GetTintColor)
					]
				// 按钮上文字
					+ SOverlay::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
							.Text(InArgs._ItemText)
							.Font(MenuStyle->Font_60)
					]
			]
	];

	// 初始化成员变量
	IsMouseButtonDown = false;
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SSDMenuIteamWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	IsMouseButtonDown = true;
	return FReply::Handled();
}

FReply SSDMenuIteamWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// 鼠标抬起触发委托
	if (IsMouseButtonDown) {
		IsMouseButtonDown = false;
		Onclicked.ExecuteIfBound(ItemType);	// 委托执行
	}

	return FReply::Handled();
}

void SSDMenuIteamWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	IsMouseButtonDown = false;
}

FSlateColor SSDMenuIteamWidget::GetTintColor() const
{
	if (IsMouseButtonDown) return FLinearColor(1.0f, 0.1f, 0.1f, 0.5f);
	return FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
}
