// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SDHandNone.h"
#include "components/BoxComponent.h"

ASDHandNone::ASDHandNone()
{
	// ����ģ��

	// ������ײ������
	AffectCollision->SetRelativeLocation(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeScale3D(FVector(0.f, 0.f, 10.f));

}

void ASDHandNone::BeginPlay()
{
	Super::BeginPlay();

	// ������Ʒ���
	ObjectIndex = 0;
}
