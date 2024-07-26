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
	// 允许开启Tick函数
	PrimaryActorTick.bCanEverTick = true;

	// 绑定控制器
	PlayerControllerClass = ASDPlayerController::StaticClass();
	
	// 绑定UI，通过HUDClass添加UI组件到界面
	HUDClass = ASDGameHUD::StaticClass();

	// 绑定玩家状态类
	PlayerStateClass = ASDPlayerState::StaticClass();

	// 绑定人物类
	DefaultPawnClass = ASDPlayerCharacter::StaticClass();

	// 开始需要初始化背包
	IsInitPackage = false;
}

void ASDGameMode::Tick(float DeltaSeconds)
{
	// 初始化及更新小地图
	InitializeMiniMapCamera();

	// 初始化及更新背包
	InitializePackage();
}

void ASDGameMode::InitGamePlayModule()
{
	// 添加引用,初始化指针
	SPController = Cast<ASDPlayerController>(UGameplayStatics::GetPlayerController(GWorld, 0));
	SPCharacter = Cast<ASDPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GWorld, 0));
	if (SPController != nullptr)SPState = Cast<ASDPlayerState>(SPController->PlayerState);

}

void ASDGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 游戏数据初始化
	SDDataHandle::Get()->InitGameData();

	if (!SPController) InitGamePlayModule();

}

void ASDGameMode::InitializePackage()
{
	if (IsInitPackage) {
		return;
	}

	// 调用委托初始化背包
	InitPackageManager.ExecuteIfBound();

	// 绑定背包相关委托
	// 物品丢弃绑定
	SDPackageManager::Get()->PlayerThrowObject.BindUObject(SPCharacter, &ASDPlayerCharacter::PlayerThrowObject);
	// 手持物品更改绑定
	SDPackageManager::Get()->ChangeHandObject.BindUObject(SPState, &ASDPlayerState::ChangeHandObject);

	IsInitPackage = true;
}

void ASDGameMode::InitializeMiniMapCamera()
{
	// 如果小地图相机不存在则先创建
	if (!IsCreateMiniMap && GetWorld()) {
		// 生成小地图摄像机
		MiniMapCamera = GetWorld()->SpawnActor<ASDSceneCapture2D>(ASDSceneCapture2D::StaticClass());
		// 运行小地图委托，给MiniMapWidget传递渲染的MiniMapTex
		RegisterMiniMap.ExecuteIfBound(MiniMapCamera->GetMiniMapTex());
		// 绑定小地图缩放委托
		SPController->UpdateMiniMapWidth.BindUObject(MiniMapCamera, &ASDSceneCapture2D::UpdateMiniMapWidth);
		// 设置小地图已生成
		IsCreateMiniMap = true;
	}

	// 如果以及创建则每帧更新
	if (IsCreateMiniMap) {
		// 传入玩家的位置及旋转更新小地图
		MiniMapCamera->UpdateTransform(SPCharacter->GetActorLocation(), SPCharacter->GetActorRotation());

		// 更新小地图数据
		TArray<FVector2D> EnemyPosList;
		TArray<bool> EnemyLockList;
		TArray<float> EnemyRotateList;

		// 获取场景中的敌人
		for (TActorIterator<ASDEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt) {
			// 获取敌人相对于玩家的位置
			FVector EnemyPos = FVector((*EnemyIt)->GetActorLocation().X - SPCharacter->GetActorLocation().X, (*EnemyIt)->GetActorLocation().Y - SPCharacter->GetActorLocation().Y, 0.f);
			// 通过四元数获取敌人的世界坐标实际位置，再绕角色方位进行旋转
			EnemyPos = FQuat(FVector::UpVector, FMath::DegreesToRadians(-SPCharacter->GetActorRotation().Yaw - 90.f)) * EnemyPos;
			// 去掉z轴
			EnemyPosList.Add(FVector2D(EnemyPos.X, EnemyPos.Y));

			EnemyLockList.Add((*EnemyIt)->IsLockPlayer());
			// 获取敌人相对于玩家的旋转
			EnemyRotateList.Add((*EnemyIt)->GetActorRotation().Yaw - SPCharacter->GetActorRotation().Yaw);	
		}

		// 每帧更新小地图方向及文字位置
		UpdateMapData.ExecuteIfBound(SPCharacter->GetActorRotation(), MiniMapCamera->GetMapSize(), &EnemyPosList, &EnemyLockList, &EnemyRotateList);
	}
}
