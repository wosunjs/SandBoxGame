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

// ��ʼ������������ί��
DECLARE_DELEGATE(FInitPackageManager)
// ע��MiniMap����ͼ�Ͳ���
DECLARE_DELEGATE_OneParam(FRegisterMiniMap, UTextureRenderTarget2D*)
// ע�����MiniMap���ݵ�ί������
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

	// ��дTick����
	virtual void Tick(float DeltaSeconds);

	// �����ֵ����GameHUD���ã����������
	void InitGamePlayModule();

public:
	// ������Ϸ����GameMode�ƿ���Щָ��
	ASDPlayerController* SPController;
	
	ASDPlayerCharacter* SPCharacter;

	ASDPlayerState* SPState;
	
	// ��ʼ����������ί�У��󶨵ķ�����PackageWidget��InitPackageManager����
	FInitPackageManager InitPackageManager;

	// С��ͼע��ί��
	FRegisterMiniMap RegisterMiniMap;

	// ����С��ͼ���ݸ���ί��(�޸�С��ͼ����),�󶨵ķ�����MiniMapWidget��UpdateMapDirection
	FUpdateMapData UpdateMapData;

protected:
	virtual void BeginPlay() override;

	// ��ʼ������������
	void InitializePackage();

	// ��ʼ���Լ�����С��ͼ
	void InitializeMiniMapCamera();

private:
	// �Ƿ��Ѿ���ʼ������
	bool IsInitPackage;

	// С��ͼ���ָ��
	ASDSceneCapture2D* MiniMapCamera;

	// �Ƿ��Ѿ�����С��ͼ�����
	bool IsCreateMiniMap;
};
