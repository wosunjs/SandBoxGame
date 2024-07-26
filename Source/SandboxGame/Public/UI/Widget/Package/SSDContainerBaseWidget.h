// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/SDTypes.h"

struct FSDGameStyle;

// 合成输入委托
DECLARE_DELEGATE(FCompoundInput)
// 合成提取委托，参数是物品序号，物品数量
DECLARE_DELEGATE_TwoParams(FCompoundOutput, int, int)
// 丢弃物品委托，参数是物品序号，物品数量
DECLARE_DELEGATE_TwoParams(FThrowObject, int, int)
// 背包快捷栏更新状态引起游戏变化委托，参数分别是快捷栏序列号，更新的物品ID，更新物品数量
DECLARE_DELEGATE_ThreeParams(FPackageShortChange, int, int, int)

/**
 * 
 */
class SANDBOXGAME_API SSDContainerBaseWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDContainerBaseWidget)
	{}
	// 使子类的参数作用于父类
	SLATE_ATTRIBUTE(int, WorkIndex)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// 获取实例
	static TSharedPtr<SSDContainerBaseWidget> CreateContainer(EContainerType::Type NeedType, int WorkID);

	// 设置鼠标移动到容器上的状态
	void UpdateHovered(bool IsHovered);

	// 重置自身属性
	virtual void ResetContainerPara(int ObjectID, int Num);

	// 获取容器中物品ID及数量
	int GetIndex() const;
	int GetNum() const;

	// 左键点击操作（传入ID和Num，以及点击后鼠标所持物品ID和数量）
	virtual void LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum);

	// 右键点击操作
	virtual void RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum);

	// 是否为空
	bool IsEmpty();

	// 是否有空间
	bool RemainSpace(int ObjectID);

	// 添加一个元素
	void AddObject(int ObjectID);

public:
	// 合成输入委托
	FCompoundInput CompoundInput;
	// 合成输出委托
	FCompoundOutput CompoundOutput;
	// 物品丢弃委托
	FThrowObject ThrowObject;
	// 快捷栏更新委托
	FPackageShortChange PackageShortChange;

protected:
	// 获取物品是否可以叠加
	bool MultiplyAble(int ObjectID);

protected:
	// 获取GameWidgetStyle
	const FSDGameStyle* GameStyle;

	// 工作序号,用于标定在背包中的位置
	TAttribute<int> WorkIndex;

	// 容器组件
	TSharedPtr<SBorder> ContainerBorder;
	TSharedPtr<SBorder> ObjectImage;
	TSharedPtr<STextBlock> ObjectNumText;

	// 是否在Hover状态
	bool IsHover;

	// 物品序号
	int ObjectIndex;
	// 物品数量
	int ObjectNum;
};
