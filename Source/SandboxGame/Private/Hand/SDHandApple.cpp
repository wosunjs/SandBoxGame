// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SDHandApple.h"
#include "Components/StaticMeshComponent.h"
#include "components/BoxComponent.h"

ASDHandApple::ASDHandApple()
{
	// 给模型组件添加模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT(
		"StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Item_Fruit_02.SM_Item_Fruit_02'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	// 设置模型位移和旋转
	BaseMesh->SetRelativeLocation(FVector(-8.f, -3.f, 7.f));
	BaseMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	BaseMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	// 设置碰撞盒属性
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 10.f));

}

void ASDHandApple::BeginPlay()
{
	Super::BeginPlay();

	// 设置物品序号
	ObjectIndex = 3;

}