  // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SDMenuHUD.generated.h"

class SSDMenuHUDWidget;

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	ASDMenuHUD();

private:
	TSharedPtr<SSDMenuHUDWidget> MenuHUDWidget;

};
