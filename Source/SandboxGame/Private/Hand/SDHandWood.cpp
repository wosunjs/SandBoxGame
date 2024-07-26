// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SDHandWood.h"
#include "Components/StaticMeshComponent.h"
#include "components/BoxComponent.h"

ASDHandWood::ASDHandWood()
{
	// 给模型组件添加模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT(
		"StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_TreeLog_01.SM_Env_TreeLog_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	// 设置模型位移和旋转
	BaseMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));

	// 设置碰撞盒属性
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(0.f, 2.f, 5.f));
}

void ASDHandWood::BeginPlay()
{
	Super::BeginPlay();

	// 设置物品序号
	ObjectIndex = 1;

}
