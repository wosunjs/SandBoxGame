// Copyright Epic Games, Inc. All Rights Reserved.

#include "SandboxGame.h"
#include "Modules/ModuleManager.h"
#include "UI/Style/SDStyle.h"


void FSandboxGameModule::StartupModule()
{
	// 先取消注册避免先加载再卸载再加载导致的样式重复注册
	FSlateStyleRegistry::UnRegisterSlateStyle(SDStyle::GetStyleSetName());
	// 初始化样式
	SDStyle::Initialize();
}

void FSandboxGameModule::ShutdownModule()
{
	SDStyle::ShutDown();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSandboxGameModule, SandboxGame, "SandboxGame");