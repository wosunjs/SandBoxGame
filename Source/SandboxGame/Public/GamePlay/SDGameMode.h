// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SDGameMode.generated.h"

class ASDPlayerController;
class ASDPlayerCharacter;
class ASDPlayerState;
class ASDSceneCapture2D;
class UTextureRenderTarget2D;

// 初始化背包管理器委托
DECLARE_DELEGATE(FInitPackageManager)
// 注册MiniMap的贴图和材质
DECLARE_DELEGATE_OneParam(FRegisterMiniMap, UTextureRenderTarget2D*)
// 注册更新MiniMap数据的委托声明
DECLARE_DELEGATE_FiveParams(FUpdateMapData, const FRotator, const float, const TArray<FVector2D>*, const TArray<bool>*, const TArray<float>*)

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASDGameMode();

	// 重写Tick函数
	virtual void Tick(float DeltaSeconds);

	// 组件赋值，给GameHUD调用，避免空引用
	void InitGamePlayModule();

public:
	// 单人游戏中由GameMode掌控这些指针
	ASDPlayerController* SPController;
	
	ASDPlayerCharacter* SPCharacter;

	ASDPlayerState* SPState;
	
	// 初始化背包管理委托，绑定的方法是PackageWidget的InitPackageManager方法
	FInitPackageManager InitPackageManager;

	// 小地图注册委托
	FRegisterMiniMap RegisterMiniMap;

	// 声明小地图数据更新委托(修改小地图数据),绑定的方法是MiniMapWidget的UpdateMapDirection
	FUpdateMapData UpdateMapData;

protected:
	virtual void BeginPlay() override;

	// 初始化背包管理器
	void InitializePackage();

	// 初始化以及更新小地图
	void InitializeMiniMapCamera();

private:
	// 是否已经初始化背包
	bool IsInitPackage;

	// 小地图相机指针
	ASDSceneCapture2D* MiniMapCamera;

	// 是否已经生成小地图摄像机
	bool IsCreateMiniMap;
};
