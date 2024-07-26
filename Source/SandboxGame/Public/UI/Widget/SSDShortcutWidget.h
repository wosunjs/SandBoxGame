// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SDTypes.h"

#include "Widgets/SCompoundWidget.h"

struct FSDGameStyle;
class STextBlock;
class SUniformGridPanel;

// 注册容器到PlayerState类的委托
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
	// 创建委托变量
	FRegisterShortcutContainer RegisterShortcutContainer; 

private:
	// 初始化所有容器
	void InitContainer();

private:
	// 获取GameWidgetStyle
	const FSDGameStyle* GameStyle;

	// 物品名字指针
	TSharedPtr<STextBlock> shortcutInfoTextBlock;

	// 网格指针
	TSharedPtr<SUniformGridPanel> GridPanel;

	// 是否初始化容器
	bool IsInitContainer;

};
