// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"


// 前向声明
class FSlateStyleSet;


/**
 * 
 */
class SANDBOXGAME_API SDStyle
{
public:
	// 获取控件样式并反射给ue

	static void Initialize();

	static FName GetStyleSetName();		// 获取样式在游戏中注册的名字

	static void ShutDown();		// 游戏关闭时取消注册

	static const ISlateStyle& Get();	// 外部获取单例接口

private:

	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> SDStyleInstance;	// 单例指针




};
