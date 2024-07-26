// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SDTypes.h"

#include "Widgets/SCompoundWidget.h"

struct FSDGameStyle;
class STextBlock;
class SUniformGridPanel;

// ע��������PlayerState���ί��
DECLARE_DELEGATE_TwoParams(FRegisterShortcutContainer, TArray<TSharedPtr<ShortcutContainer>>*, TSharedPtr<STextBlock>)

/**
 * 
 */
class SANDBOXGAME_API SSDShortcutWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDShortcutWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

public:
	// ����ί�б���
	FRegisterShortcutContainer RegisterShortcutContainer; 

private:
	// ��ʼ����������
	void InitContainer();

private:
	// ��ȡGameWidgetStyle
	const FSDGameStyle* GameStyle;

	// ��Ʒ����ָ��
	TSharedPtr<STextBlock> shortcutInfoTextBlock;

	// ����ָ��
	TSharedPtr<SUniformGridPanel> GridPanel;

	// �Ƿ��ʼ������
	bool IsInitContainer;

};
