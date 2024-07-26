// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource/SDResourceRock.h"
#include "Components/StaticMeshComponent.h"

ASDResourceRock::ASDResourceRock()
{
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_02.SM_Env_Rock_02'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_03.SM_Env_Rock_03'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_03_Snow.SM_Env_Rock_03_Snow'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_04.SM_Env_Rock_04'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_04_Snow.SM_Env_Rock_04_Snow'"));

	// 产生新的随机种子
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	// 随机选择一个模型生成资源外型
	int RandIndex = Stream.RandRange(0, ResourcePath.Num() - 1);

	// 给模型组件添加上模型，这里不能使用静态变量
	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(*ResourcePath[RandIndex]);
	// 绑定模型到Mesh组件
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	ResourceIndex = 2;
}
