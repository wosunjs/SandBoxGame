// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FSDGameStyle;
class SProgressBar;

/**
 * 
 */
class SANDBOXGAME_API SSDPlayerStateWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDPlayerStateWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// 更新状态事件，绑定的委托是PlayerState的UpdateStateWidget
	void UpdateStateWidget(float HPValue, float HungerValue);

private:
	// 获取GameWidgetStyle
	const FSDGameStyle* GameStyle;

	// 血量
	TSharedPtr<SProgressBar> HPBar;

	// 饥饿值
	TSharedPtr<SProgressBar> HungerBar;


};
