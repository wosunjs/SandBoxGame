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

	// �����µ��������
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	// ���ѡ��һ��ģ��������Դ����
	int RandIndex = Stream.RandRange(0, ResourcePath.Num() - 1);

	// ��ģ����������ģ�ͣ����ﲻ��ʹ�þ�̬����
	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(*ResourcePath[RandIndex]);
	// ��ģ�͵�Mesh���
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	ResourceIndex = 2;
}
