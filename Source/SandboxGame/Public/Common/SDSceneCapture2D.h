// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "SDSceneCapture2D.generated.h"

class UTextureRenderTarget2D;

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDSceneCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()
	
public:
	ASDSceneCapture2D();

	// 获取MiniMapText
	UTextureRenderTarget2D* GetMiniMapTex();

	// 更新变化(通过玩家的位置和旋转),在GameMode中调用，以次来不断更新小地图
	void UpdateTransform(FVector NormLocation, FRotator NormRotator);

	// 更新地图大小
	void UpdateMiniMapWidth(int Delta);

	// 获取小地图尺寸
	float GetMapSize();

private:
	UTextureRenderTarget2D* MiniMapTex;
};
