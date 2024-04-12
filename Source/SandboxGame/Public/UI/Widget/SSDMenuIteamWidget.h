// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/SDTypes.h"

// 定义一个传入按钮类型，名为FIteamClicked的委托
DECLARE_DELEGATE_OneParam(FIteamClicked, const EMenuItem::Type)

/**
 * 
 */
class SANDBOXGAME_API SSDMenuIteamWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDMenuIteamWidget)
	{}

	// 用宏给控件添加变量，（类型， 名称）
	SLATE_ATTRIBUTE(FText, ItemText)

	SLATE_ATTRIBUTE(EMenuItem::Type, ItemType)

	// 用宏给控件添加响应事件，（委托名， 事件参数名称），每次点击便会运行Onclick绑定的函数
	SLATE_EVENT(FIteamClicked, Onclicked)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// 重写鼠标对组件造成影响的三种方法:点击、放下、离开
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

private:
	// 获取按钮按下后的颜色
	FSlateColor GetTintColor() const;

private:
	// 按下后触发的事件委托（通过Construct将以上SLATE定义的数据通过InArgs传入完成初始化）
	FIteamClicked Onclicked;

	// 按钮类型
	EMenuItem::Type ItemType;

	// 获取Menu样式
	const struct FSDMenuStyle* MenuStyle;

	// 按钮是否已经按下
	bool IsMouseButtonDown;

};
