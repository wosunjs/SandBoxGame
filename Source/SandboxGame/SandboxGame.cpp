// Copyright Epic Games, Inc. All Rights Reserved.

#include "SandboxGame.h"
#include "Modules/ModuleManager.h"
#include "UI/Style/SDStyle.h"


void FSandboxGameModule::StartupModule()
{
	// ��ȡ��ע������ȼ�����ж���ټ��ص��µ���ʽ�ظ�ע��
	FSlateStyleRegistry::UnRegisterSlateStyle(SDStyle::GetStyleSetName());
	// ��ʼ����ʽ
	SDStyle::Initialize();
}

void FSandboxGameModule::ShutdownModule()
{
	SDStyle::ShutDown();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSandboxGameModule, SandboxGame, "SandboxGame");