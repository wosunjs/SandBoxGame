// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"


// ǰ������
class FSlateStyleSet;


/**
 * 
 */
class SANDBOXGAME_API SDStyle
{
public:
	// ��ȡ�ؼ���ʽ�������ue

	static void Initialize();

	static FName GetStyleSetName();		// ��ȡ��ʽ����Ϸ��ע�������

	static void ShutDown();		// ��Ϸ�ر�ʱȡ��ע��

	static const ISlateStyle& Get();	// �ⲿ��ȡ�����ӿ�

private:

	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> SDStyleInstance;	// ����ָ��




};
