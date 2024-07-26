// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDResourceObject.generated.h"

UCLASS()
class SANDBOXGAME_API ASDResourceObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDResourceObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 获取物品信息
	FText GetInfoText() const;

	// 获取资源类型
	EResourceType::Type GetResourceType();

	// 获取血量百分比
	float GetHPRange();

	// 获取伤害
	ASDResourceObject* TakeObjectDamage(int Damage);

public:
	// 资源ID
	int ResourceIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 生成掉落物
	void CreateFlobObject();

protected:
	// 根组件
	USceneComponent* RootScene;

	// 静态模型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SD")
	UStaticMeshComponent* BaseMesh;

	// 保存资源地址，用于随机存储资源
	TArray<FString> ResourcePath;

	// 保存当前资源属性信息
	TSharedPtr<ResourceAttribute> ResourceAttr;

	// 血量
	int HP;

	// 基础血量
	int BaseHP;

};
