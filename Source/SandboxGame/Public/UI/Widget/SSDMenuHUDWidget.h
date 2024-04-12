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
	void Construct(const FArguments& InArgs);	// ʵ�����ؼ�

private:
	// ��UIScaler�ĺ���
	float GetUIScaler() const;

	// ��ȡ��Ļ�ĳߴ�
	FVector2D GetViewportSize() const;

	

private:
	// ��ȡMenu��ʽ
	const struct FSDMenuStyle* MenuStyle;

	// DPI����ϵ��
	TAttribute<float> UIScaler;

	// ��ȡImage��Slot
	SOverlay::FOverlaySlot* ImageSlot;

	// �˵�ָ��
	TSharedPtr<SSDMenuWidget> MenuWidget;
};
