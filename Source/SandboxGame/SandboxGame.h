// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


class FSandboxGameModule : public FDefaultGameModuleImpl
{
public:

	// ��ǰģ�����ʱ��ɾ�̬�����ĳ�ʼ��
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};

