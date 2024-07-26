// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDPickupObject.generated.h"

UCLASS()
class SANDBOXGAME_API ASDPickupObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDPickupObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 获取物品信息
	FText GetInfoText() const;

	// 物品被拾取,返回物品ID
	int TakePickup();


public:
	int ObjectIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// 根组件
	USceneComponent* RootScene;

	// 静态模型
	UStaticMeshComponent* BaseMesh;
	

};
