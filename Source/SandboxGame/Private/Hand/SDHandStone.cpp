// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SDHandStone.h"
#include "Components/StaticMeshComponent.h"
#include "components/BoxComponent.h"

ASDHandStone::ASDHandStone()
{
	// 给模型组件添加模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT(
		"StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_StoneBlock_01.SM_Prop_StoneBlock_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	// 设置模型位移和旋转
	BaseMesh->SetRelativeLocation(FVector(1.f, -1.414f, 7.071f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, 0.f, -135.f));
	BaseMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	// 设置碰撞盒属性
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(0.f, -2.f, 6.f));

}

void ASDHandStone::BeginPlay()
{
	Super::BeginPlay();

	// 设置物品序号
	ObjectIndex = 2;

}
