// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FSDGameStyle;

// 射线检测委托
DECLARE_DELEGATE_OneParam(FRegisterRayInfoEvent, TSharedPtr<STextBlock>)

/**
 * 
 */
class SANDBOXGAME_API SSDRayInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDRayInfoWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

public:
	FRegisterRayInfoEvent RegisterRayInfoEvent;

private:
	// 获取GameWidgetStyle
	const FSDGameStyle* GameStyle;

	// 保存显示射线信息的文本
	TSharedPtr<STextBlock> RayInfoTextBlock;

	// 是否以及初始化事件
	bool IsInitRayInfoEvent;
};
