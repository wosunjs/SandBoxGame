// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Style/SDStyle.h"
#include <Slate/SlateGameResources.h>

TSharedPtr<FSlateStyleSet> SDStyle::SDStyleInstance = NULL;

void SDStyle::Initialize()
{
	if (!SDStyleInstance.IsValid()) {
		SDStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*SDStyleInstance);	// ����ʽע��
	}
}

FName SDStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SDStyle"));
	return StyleSetName;
}

void SDStyle::ShutDown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*SDStyleInstance);	// ȡ��ע��
	
	// �������ü���������ʵ��
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
		"/Game/UI/Style", "/Game/UI/Style");	// ��Game/UI/Style���ҵ�StyleSetName��Ӧ����ʽ��ͼ����������

	StyleRef->Set("MenuItemFont", FSlateFontInfo("Slate/Fonts/SourceHanSans.ttf", 50));	// ��������
	
	return StyleRef;
}
