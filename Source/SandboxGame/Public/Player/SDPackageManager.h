// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SDTypes.h"
#include "UI/Widget/Package/SSDContainerBaseWidget.h"

/**
 * 
 */
class SANDBOXGAME_API SDPackageManager
{
public:
	SDPackageManager();
	
	static void Initialize();

	static TSharedPtr<SDPackageManager> Get();

	// 添加容器
	void InsertContainer(TSharedPtr<SSDContainerBaseWidget> Container, EContainerType::Type InsertType);

	// 更新悬停的容器颜色
	void UpdateHovered(FVector2D MousePos, FGeometry PackGeo);

	// 左键事件，参数是鼠标位置和PackageWidget的Geometry
	void LeftOption(FVector2D MousePos, FGeometry PackGeo);

	// 右键事件，参数是鼠标位置和PackageWidget的Geometry
	void RightOption(FVector2D MousePos, FGeometry PackGeo);

	// 检索是否可装入该物品
	bool SearchFreeSpace(int ObjectID);

	// 添加物品
	void AddObject(int ObjectID);

	// 吃东西，传入快捷栏ID，传回是否成功吃掉
	bool EatUpEvent(int shortcutID);

public:
	// 鼠标持有物品ID
	int ObjectIndex;

	// 鼠标持有物品数量
	int ObjectNum;

	// 丢弃物品委托变量，绑定的方法是PlayerState的PlayerThrowObject
	FThrowObject PlayerThrowObject;

	// 修改快捷栏委托变量
	FPackageShortChange ChangeHandObject;

private:
	// 创建实例
	static TSharedRef<SDPackageManager> Create();

	// 获取鼠标指向的容器
	TSharedPtr<SSDContainerBaseWidget> LocateContainer(FVector2D MousePos, FGeometry PackGeo);

	// 丢弃物品事件
	void ThrowObject(int ObjectID, int Num);

	// 合成提取事件
	void CompoundOutput(int ObjectID, int Num);

	// 合成输入事件
	void CompoundInput();

	// 快捷栏变更事件
	void PackageShortChange(int ShortcutID, int ObjectID, int Num);

	// 获取物品是否可以叠加
	bool MultiplyAble(int ObjectID);

	// 获取可放入物品的空间
	bool SearchFreeSpace(int ObjectID, TSharedPtr<SSDContainerBaseWidget>& FreeContainer);

private:
	// 单例指针
	static TSharedPtr<SDPackageManager> PackageInstance;

	// 容器列表
	TArray<TSharedPtr<SSDContainerBaseWidget>> InputContainerList;
	TArray<TSharedPtr<SSDContainerBaseWidget>> NormalContainerList;
	TArray<TSharedPtr<SSDContainerBaseWidget>> ShortcutContainerList;
	// 输出容器
	TSharedPtr<SSDContainerBaseWidget> OutputContainer;

	// 上一悬停容器
	TSharedPtr<SSDContainerBaseWidget> LastHoveredCon;
};
