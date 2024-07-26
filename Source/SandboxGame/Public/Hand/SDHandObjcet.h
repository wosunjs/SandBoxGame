// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDHandObjcet.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;


UCLASS()
class SANDBOXGAME_API ASDHandObjcet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDHandObjcet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 根据物品ID返回物品的静态工厂
	static TSubclassOf<AActor> SpawnHandObject(int ObjectID);

	// 是否允许碰撞检测
	void ChangeOverlayDetect(bool IsOpen);

public:
	// 物品ID
	int ObjectIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	
	// 打到物品时的音效
	USoundWave* OverlaySound;
};
