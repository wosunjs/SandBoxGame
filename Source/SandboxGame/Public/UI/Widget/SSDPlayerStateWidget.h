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

	// ����״̬�¼����󶨵�ί����PlayerState��UpdateStateWidget
	void UpdateStateWidget(float HPValue, float HungerValue);

private:
	// ��ȡGameWidgetStyle
	const FSDGameStyle* GameStyle;

	// Ѫ��
	TSharedPtr<SProgressBar> HPBar;

	// ����ֵ
	TSharedPtr<SProgressBar> HungerBar;


};
