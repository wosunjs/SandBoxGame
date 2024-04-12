// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SDMenuController.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDMenuController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ASDMenuController();

protected:

	virtual void BeginPlay() override;

};
