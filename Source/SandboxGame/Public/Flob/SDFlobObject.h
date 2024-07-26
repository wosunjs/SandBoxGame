// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDFlobObject.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTexture;
class UMaterialInstanceDynamic;
class ASDPlayerCharacter;

UCLASS()
class SANDBOXGAME_API ASDFlobObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDFlobObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 生成掉落物品
	void CreateFlobObject(int ObjectID);

	// 丢弃物品初始化，传入丢弃物品ID和丢弃方向
	void ThrowFlobObject(int ObjectID, float DirYaw);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 渲染贴图
	void RenderTexture();

	// 动态检测事件
	void DetectPlayer();

	// 销毁事件
	void DestroyEvent();

private:
	UBoxComponent* BoxCollision;

	UStaticMeshComponent* BaseMesh;

	// 掉落物品ID
	int ObjectIndex;

	UTexture* ObjectIconTex;

	UMaterialInstanceDynamic* ObjectIconMatDynamic;

	// 玩家指针
	ASDPlayerCharacter* SPCharacter;

	// 动态检测Timer
	FTimerHandle DetectTimer;

	// 定时销毁Timer
	FTimerHandle DestoryTimer;

};
