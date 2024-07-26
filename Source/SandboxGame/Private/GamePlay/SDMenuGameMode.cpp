// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/SDMenuGameMode.h"
#include "GamePlay/SDMenuController.h"
#include "UI/HUD/SDMenuHUD.h"

#include "GamePlay/SDGameInstance.h"
#include "Kismet/GameplayStatics.h"

ASDMenuGameMode::ASDMenuGameMode()
{
	// 根据MenuGameMode绑定在Map的游戏模式上，Map通过初始化MenuGameMode创建Menu菜单
	PlayerControllerClass = ASDMenuController::StaticClass();
	HUDClass = ASDMenuHUD::StaticClass();

}

void ASDMenuGameMode::BeginPlay()
{
	// 修改游戏名
	Cast<USDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GameName = FString("SDGame");

}
