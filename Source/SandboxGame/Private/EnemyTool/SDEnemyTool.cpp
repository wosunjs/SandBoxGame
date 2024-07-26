// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTool/SDEnemyTool.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"

#include "EnemyTool/SDEnemyWeapon.h"
#include "EnemyTool/SDEnemySheild.h"

#include "Player/SDPlayerCharacter.h"

// Sets default values
ASDEnemyTool::ASDEnemyTool()
{
 	// ʵ�������ڵ�
	RootScene = CreateDefaultSubobject<USceneComponent>("RootScene");
	RootComponent = RootScene;

	// ʵ����ģ��
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("NoCollision"));

	// ʵ������ײ���
	AffectCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AffectCollision"));
	AffectCollision->SetupAttachment(BaseMesh);
	AffectCollision->SetCollisionProfileName(FName("EnemyToolProfile"));
	// ��ʼʱ�ر�Overlay���
	AffectCollision->SetGenerateOverlapEvents(false);
	
	// �󶨽������Ŀ�����ر�
	FScriptDelegate OverlayBegin;
	OverlayBegin.BindUFunction(this, "OnOverlayBegin");
	AffectCollision->OnComponentBeginOverlap.Add(OverlayBegin);

	FScriptDelegate OverlayEnd;
	OverlayEnd.BindUFunction(this, "OnOverlayEnd");
	AffectCollision->OnComponentEndOverlap.Add(OverlayEnd);	// �󶨼�ⷽ������ײ��

}

void ASDEnemyTool::ChangeOverlayDetect(bool IsOpen)
{
	AffectCollision->SetGenerateOverlapEvents(IsOpen);
}

TSubclassOf<AActor> ASDEnemyTool::SpawnEnemyWeapon()
{
	return ASDEnemyWeapon::StaticClass();
}

TSubclassOf<AActor> ASDEnemyTool::SpawnEnemySheild()
{
	return ASDEnemySheild::StaticClass();
}

void ASDEnemyTool::OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASDPlayerCharacter>(OtherActor)) {
		Cast<ASDPlayerCharacter>(OtherActor)->TakeDamage(5);
	}
}

void ASDEnemyTool::OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


