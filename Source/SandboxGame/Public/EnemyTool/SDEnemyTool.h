// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDEnemyTool.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;

UCLASS()
class SANDBOXGAME_API ASDEnemyTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDEnemyTool();

	// 是否允许检测
	void ChangeOverlayDetect(bool IsOpen);

	// 创建武器/盾牌的工厂
	static TSubclassOf<AActor> SpawnEnemyWeapon();
	static TSubclassOf<AActor> SpawnEnemySheild();

protected:
	// 交互检测的函数
	UFUNCTION()
	virtual void OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// 根组件
	USceneComponent* RootScene;

	// 静态模型
	UPROPERTY(EditAnywhere, Category = "SD")
	UStaticMeshComponent* BaseMesh;

	// 碰撞包围盒
	UPROPERTY(EditAnywhere, Category = "SD")
	UBoxComponent* AffectCollision;


};
