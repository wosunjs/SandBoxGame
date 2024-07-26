// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/SDMenuGameMode.h"
#include "GamePlay/SDMenuController.h"
#include "UI/HUD/SDMenuHUD.h"

#include "GamePlay/SDGameInstance.h"
#include "Kismet/GameplayStatics.h"

ASDMenuGameMode::ASDMenuGameMode()
{
	// ����MenuGameMode����Map����Ϸģʽ�ϣ�Mapͨ����ʼ��MenuGameMode����Menu�˵�
	PlayerControllerClass = ASDMenuController::StaticClass();
	HUDClass = ASDMenuHUD::StaticClass();

}

void ASDMenuGameMode::BeginPlay()
{
	// �޸���Ϸ��
	Cast<USDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GameName = FString("SDGame");

}
