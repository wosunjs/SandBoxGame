// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SDHandWood.h"
#include "Components/StaticMeshComponent.h"
#include "components/BoxComponent.h"

ASDHandWood::ASDHandWood()
{
	// ��ģ��������ģ��
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT(
		"StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_TreeLog_01.SM_Env_TreeLog_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	// ����ģ��λ�ƺ���ת
	BaseMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));

	// ������ײ������
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(0.f, 2.f, 5.f));
}

void ASDHandWood::BeginPlay()
{
	Super::BeginPlay();

	// ������Ʒ���
	ObjectIndex = 1;

}
