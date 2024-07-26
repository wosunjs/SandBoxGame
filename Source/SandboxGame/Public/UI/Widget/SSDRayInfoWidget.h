// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FSDGameStyle;

// ���߼��ί��
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
	// ��ȡGameWidgetStyle
	const FSDGameStyle* GameStyle;

	// ������ʾ������Ϣ���ı�
	TSharedPtr<STextBlock> RayInfoTextBlock;

	// �Ƿ��Լ���ʼ���¼�
	bool IsInitRayInfoEvent;
};
