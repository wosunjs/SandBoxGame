// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SDMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASDMenuGameMode();

protected:
	virtual void BeginPlay() override;


};