// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


class FSandboxGameModule : public FDefaultGameModuleImpl
{
public:

	// 当前模组加载时完成静态变量的初始化
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};

