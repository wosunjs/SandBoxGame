// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SOverlay.h"


class SSDMenuWidget;

/**
 * 
 */
class SANDBOXGAME_API SSDMenuHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDMenuHUDWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);	// 实例化控件

private:
	// 绑定UIScaler的函数
	float GetUIScaler() const;

	// 获取屏幕的尺寸
	FVector2D GetViewportSize() const;

	

private:
	// 获取Menu样式
	const struct FSDMenuStyle* MenuStyle;

	// DPI缩放系数
	TAttribute<float> UIScaler;

	// 获取Image的Slot
	SOverlay::FOverlaySlot* ImageSlot;

	// 菜单指针
	TSharedPtr<SSDMenuWidget> MenuWidget;
};
