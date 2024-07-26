// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SDHandObjcet.h"
#include "Components/StaticMeshComponent.h"
#include "components/BoxComponent.h"
#include "Hand/SDHandNone.h"
#include "Hand/SDHandApple.h"
#include "Hand/SDHandAxe.h"
#include "Hand/SDHandHammer.h"
#include "Hand/SDHandMeat.h"
#include "Hand/SDHandNone.h"
#include "Hand/SDHandStone.h"
#include "Hand/SDHandSword.h"
#include "Hand/SDHandWood.h"
#include "Common/SDHelper.h"

#include "Data/SDDataHandle.h"
#include "Enemy/SDEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASDHandObjcet::ASDHandObjcet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 实例化根组件
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;


	// 创建静态模型组件
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("NoCollision"));

	// 实例化碰撞组件
	AffectCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AffectCollision"));
	AffectCollision->SetupAttachment(RootComponent);
	AffectCollision->SetCollisionProfileName(FName("ToolProfile"));

	// 初始时关闭Overlay检测
	AffectCollision->SetGenerateOverlapEvents(false);

	// 绑定检测函数到碰撞体
	FScriptDelegate OverlayBegin;
	OverlayBegin.BindUFunction(this, "OnOverlayBegin");
	AffectCollision->OnComponentBeginOverlap.Add(OverlayBegin);

	FScriptDelegate OverlayEnd;
	OverlayEnd.BindUFunction(this, "OnOverlayEnd");
	AffectCollision->OnComponentBeginOverlap.Add(OverlayEnd);

	// 默认攻击音效绑定为拳击
	static ConstructorHelpers::FObjectFinder<USoundWave> StaticSound(TEXT("SoundWave'/Game/Res/Sound/GameSound/Punch.Punch'"));
	OverlaySound = StaticSound.Object;

}

// Called when the game starts or when spawned
void ASDHandObjcet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASDHandObjcet::OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 交互检测
	if (Cast<ASDEnemyCharacter>(OtherActor)) {
		// 获取物品属性
		TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
		// 获取对生物的伤害
		Cast<ASDEnemyCharacter>(OtherActor)->UnderAttack(ObjectAttr->AnimalAttack);
	}
	// 如果打击音效存在则播放1 
	if (OverlaySound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), OverlaySound, OtherActor->GetActorLocation());
}

void ASDHandObjcet::OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

// Called every frame
void ASDHandObjcet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TSubclassOf<AActor> ASDHandObjcet::SpawnHandObject(int ObjectID)
{
	switch (ObjectID)
	{
	case 0:
		return ASDHandNone::StaticClass();
	case 1:
		return ASDHandWood::StaticClass();
	case 2:
		return ASDHandStone::StaticClass();
	case 3:
		return ASDHandApple::StaticClass();
	case 4:
		return ASDHandMeat::StaticClass();
	case 5:
		return ASDHandAxe::StaticClass();
	case 6:
		return ASDHandHammer::StaticClass();
	case 7:
		return ASDHandSword::StaticClass();
	}

	return ASDHandNone::StaticClass();
}

void ASDHandObjcet::ChangeOverlayDetect(bool IsOpen)
{
	AffectCollision->SetGenerateOverlapEvents(IsOpen);
}

