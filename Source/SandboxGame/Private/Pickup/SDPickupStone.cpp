// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup/SDPickupStone.h"

ASDPickupStone::ASDPickupStone()
{
	// 给模型组件添加模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT(
		"StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_StoneBlock_01.SM_Prop_StoneBlock_01'"));
	// 绑定模型到Mesh组件
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.5f));

	ObjectIndex = 2;
}
