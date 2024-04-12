// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Style/SDStyle.h"
#include <Slate/SlateGameResources.h>

TSharedPtr<FSlateStyleSet> SDStyle::SDStyleInstance = NULL;

void SDStyle::Initialize()
{
	if (!SDStyleInstance.IsValid()) {
		SDStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*SDStyleInstance);	// 将样式注册
	}
}

FName SDStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SDStyle"));
	return StyleSetName;
}

void SDStyle::ShutDown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*SDStyleInstance);	// 取消注册
	
	// 减少引用计数以销毁实例
	ensure(SDStyleInstance.IsUnique());
	SDStyleInstance.Reset();

}

const ISlateStyle& SDStyle::Get()
{
	return *SDStyleInstance;
}

TSharedRef<FSlateStyleSet> SDStyle::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(SDStyle::GetStyleSetName(), 
		"/Game/UI/Style", "/Game/UI/Style");	// 在Game/UI/Style中找到StyleSetName对应的样式蓝图并创建引用

	StyleRef->Set("MenuItemFont", FSlateFontInfo("Slate/Fonts/SourceHanSans.ttf", 50));	// 定义字体
	
	return StyleRef;
}
