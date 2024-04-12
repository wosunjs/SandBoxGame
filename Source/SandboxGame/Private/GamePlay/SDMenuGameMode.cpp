// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/SDMenuGameMode.h"
#include "GamePlay/SDMenuController.h"
#include "UI/HUD/SDMenuHUD.h"


ASDMenuGameMode::ASDMenuGameMode()
{
	PlayerControllerClass = ASDMenuController::StaticClass();
	HUDClass = ASDMenuHUD::StaticClass();

}
