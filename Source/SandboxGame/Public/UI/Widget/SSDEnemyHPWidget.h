// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SProgressBar;

/**
 * 
 */
class SANDBOXGAME_API SSDEnemyHPWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDEnemyHPWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// Ѫ���޸Ľӿ�
	void ChangeHP(float HP);

private:
	TSharedPtr<SProgressBar> HPBar;

	// Ѫ����ɫ
	FLinearColor ResultColor;
};
