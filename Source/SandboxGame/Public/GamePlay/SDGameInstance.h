// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	//virtual void Init() override;	// 在世界运行之前运行的Init方法

	UPROPERTY(VisibleAnywhere, Category = "SD");
	FString GameName;

};
