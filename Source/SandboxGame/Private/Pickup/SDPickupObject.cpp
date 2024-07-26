// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup/SDPickupObject.h"
#include "Data/SDDataHandle.h"
#include "Data/SDTypes.h"
#include "Engine/GameEngine.h"

// Sets default values
ASDPickupObject::ASDPickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 实例化根节点
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	// 实例化模型组件
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("PickupProfile"));


}

// Called when the game starts or when spawned
void ASDPickupObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASDPickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText ASDPickupObject::GetInfoText() const
{
	TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
	switch (SDDataHandle::Get()->GetLocalizationCulture())
	{
	case ECultureTeam::EN:
		return ObjectAttr->EN;
	case ECultureTeam::ZH:
		return ObjectAttr->ZH;
	default:
		return ObjectAttr->EN;
	}
}

int ASDPickupObject::TakePickup()
{
	BaseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	if (GetWorld()) {
		GetWorld()->DestroyActor(this);
	}
	return ObjectIndex;
}

