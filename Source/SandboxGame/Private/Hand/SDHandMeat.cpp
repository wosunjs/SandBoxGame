// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SDHandMeat.h"
#include "Components/StaticMeshComponent.h"
#include "components/BoxComponent.h"

ASDHandMeat::ASDHandMeat()
{
	// ��ģ��������ģ��
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT(
		"StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_Meat_02.SM_Prop_Meat_02'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	// ����ģ��λ�ƺ���ת
	BaseMesh->SetRelativeLocation(FVector(6.f, -7.044f, 2.62f));
	BaseMesh->SetRelativeRotation(FRotator(-50.f, 90.f, 0.f));
	BaseMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	// ������ײ������
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(5.f, 0.f, 10.f));

}

void ASDHandMeat::BeginPlay()
{
	Super::BeginPlay();

	// ������Ʒ���
	ObjectIndex = 4;

}

