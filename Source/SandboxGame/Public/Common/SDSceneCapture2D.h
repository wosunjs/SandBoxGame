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

	// ��ȡMiniMapText
	UTextureRenderTarget2D* GetMiniMapTex();

	// ���±仯(ͨ����ҵ�λ�ú���ת),��GameMode�е��ã��Դ������ϸ���С��ͼ
	void UpdateTransform(FVector NormLocation, FRotator NormRotator);

	// ���µ�ͼ��С
	void UpdateMiniMapWidth(int Delta);

	// ��ȡС��ͼ�ߴ�
	float GetMapSize();

private:
	UTextureRenderTarget2D* MiniMapTex;
};
