// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SDGameHUD.generated.h"

class SSDGameHUDWidget;
class ASDGameMode;

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ASDGameHUD();

public:
	// ±£¥ÊGameMode÷∏’Î
	ASDGameMode* GM;

protected:
	virtual void BeginPlay() override;

private:
	TSharedPtr<SSDGameHUDWidget> GameHUDWidget;
};
