// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlay/SDGameMode.h"
#include "Player/SDPlayerController.h"
#include "Player/SDPlayerCharacter.h"
#include "Player/SDPlayerState.h"
#include "Player/SDPackageManager.h"
#include "UI/HUD/SDGameHUD.h"

#include "Data/SDDataHandle.h"
#include "Common/SDHelper.h"
#include "GamePlay/SDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "Common/SDSceneCapture2D.h"

#include "Enemy/SDEnemyCharacter.h"

ASDGameMode::ASDGameMode()
{
	// ������Tick����
	PrimaryActorTick.bCanEverTick = true;

	// �󶨿�����
	PlayerControllerClass = ASDPlayerController::StaticClass();
	
	// ��UI��ͨ��HUDClass���UI���������
	HUDClass = ASDGameHUD::StaticClass();

	// �����״̬��
	PlayerStateClass = ASDPlayerState::StaticClass();

	// ��������
	DefaultPawnClass = ASDPlayerCharacter::StaticClass();

	// ��ʼ��Ҫ��ʼ������
	IsInitPackage = false;
}

void ASDGameMode::Tick(float DeltaSeconds)
{
	// ��ʼ��������С��ͼ
	InitializeMiniMapCamera();

	// ��ʼ�������±���
	InitializePackage();
}

void ASDGameMode::InitGamePlayModule()
{
	// �������,��ʼ��ָ��
	SPController = Cast<ASDPlayerController>(UGameplayStatics::GetPlayerController(GWorld, 0));
	SPCharacter = Cast<ASDPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GWorld, 0));
	if (SPController != nullptr)SPState = Cast<ASDPlayerState>(SPController->PlayerState);

}

void ASDGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ��Ϸ���ݳ�ʼ��
	SDDataHandle::Get()->InitGameData();

	if (!SPController) InitGamePlayModule();

}

void ASDGameMode::InitializePackage()
{
	if (IsInitPackage) {
		return;
	}

	// ����ί�г�ʼ������
	InitPackageManager.ExecuteIfBound();

	// �󶨱������ί��
	// ��Ʒ������
	SDPackageManager::Get()->PlayerThrowObject.BindUObject(SPCharacter, &ASDPlayerCharacter::PlayerThrowObject);
	// �ֳ���Ʒ���İ�
	SDPackageManager::Get()->ChangeHandObject.BindUObject(SPState, &ASDPlayerState::ChangeHandObject);

	IsInitPackage = true;
}

void ASDGameMode::InitializeMiniMapCamera()
{
	// ���С��ͼ������������ȴ���
	if (!IsCreateMiniMap && GetWorld()) {
		// ����С��ͼ�����
		MiniMapCamera = GetWorld()->SpawnActor<ASDSceneCapture2D>(ASDSceneCapture2D::StaticClass());
		// ����С��ͼί�У���MiniMapWidget������Ⱦ��MiniMapTex
		RegisterMiniMap.ExecuteIfBound(MiniMapCamera->GetMiniMapTex());
		// ��С��ͼ����ί��
		SPController->UpdateMiniMapWidth.BindUObject(MiniMapCamera, &ASDSceneCapture2D::UpdateMiniMapWidth);
		// ����С��ͼ������
		IsCreateMiniMap = true;
	}

	// ����Լ�������ÿ֡����
	if (IsCreateMiniMap) {
		// ������ҵ�λ�ü���ת����С��ͼ
		MiniMapCamera->UpdateTransform(SPCharacter->GetActorLocation(), SPCharacter->GetActorRotation());

		// ����С��ͼ����
		TArray<FVector2D> EnemyPosList;
		TArray<bool> EnemyLockList;
		TArray<float> EnemyRotateList;

		// ��ȡ�����еĵ���
		for (TActorIterator<ASDEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt) {
			// ��ȡ�����������ҵ�λ��
			FVector EnemyPos = FVector((*EnemyIt)->GetActorLocation().X - SPCharacter->GetActorLocation().X, (*EnemyIt)->GetActorLocation().Y - SPCharacter->GetActorLocation().Y, 0.f);
			// ͨ����Ԫ����ȡ���˵���������ʵ��λ�ã����ƽ�ɫ��λ������ת
			EnemyPos = FQuat(FVector::UpVector, FMath::DegreesToRadians(-SPCharacter->GetActorRotation().Yaw - 90.f)) * EnemyPos;
			// ȥ��z��
			EnemyPosList.Add(FVector2D(EnemyPos.X, EnemyPos.Y));

			EnemyLockList.Add((*EnemyIt)->IsLockPlayer());
			// ��ȡ�����������ҵ���ת
			EnemyRotateList.Add((*EnemyIt)->GetActorRotation().Yaw - SPCharacter->GetActorRotation().Yaw);	
		}

		// ÿ֡����С��ͼ��������λ��
		UpdateMapData.ExecuteIfBound(SPCharacter->GetActorRotation(), MiniMapCamera->GetMapSize(), &EnemyPosList, &EnemyLockList, &EnemyRotateList);
	}
}
