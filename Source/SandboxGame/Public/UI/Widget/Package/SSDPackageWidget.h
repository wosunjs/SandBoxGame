// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/SDTypes.h"


/**
 * 
 */
class SANDBOXGAME_API SSDPackageWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDPackageWidget)
	{}

	SLATE_ATTRIBUTE(float, UIScaler)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// 重写绘制函数
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, 
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, 
		int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	// 重写鼠标点击事件
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	// 注册背包管理器事件，由Playercharacter的InitPackageManager委托进行调用
	void InitPackageManager();

	
private:
	// 获取GameWidgetStyle
	const FSDGameStyle* GameStyle;

	// 快捷栏表格
	TSharedPtr<SUniformGridPanel> ShortcutGrid;

	// 背包表格
	TSharedPtr<SUniformGridPanel> PackageGrid;

	// 合成表表格
	TSharedPtr<SUniformGridPanel> CompoundGrid;

	// 输出容器
	TSharedPtr<SBorder> OutputBorder;

	// 鼠标位置
	FVector2D MousePosition;

	// DPI的缩放
	TAttribute<float> UIScaler;

	// 是否已经初始化背包管理器
	bool IsInitPackageManager;
};
