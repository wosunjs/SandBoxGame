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
 	// 实例化根节点
	RootScene = CreateDefaultSubobject<USceneComponent>("RootScene");
	RootComponent = RootScene;

	// 实例化模型
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("NoCollision"));

	// 实例化碰撞组件
	AffectCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AffectCollision"));
	AffectCollision->SetupAttachment(BaseMesh);
	AffectCollision->SetCollisionProfileName(FName("EnemyToolProfile"));
	// 初始时关闭Overlay检测
	AffectCollision->SetGenerateOverlapEvents(false);
	
	// 绑定交互检测的开启与关闭
	FScriptDelegate OverlayBegin;
	OverlayBegin.BindUFunction(this, "OnOverlayBegin");
	AffectCollision->OnComponentBeginOverlap.Add(OverlayBegin);

	FScriptDelegate OverlayEnd;
	OverlayEnd.BindUFunction(this, "OnOverlayEnd");
	AffectCollision->OnComponentEndOverlap.Add(OverlayEnd);	// 绑定检测方法到碰撞体

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


