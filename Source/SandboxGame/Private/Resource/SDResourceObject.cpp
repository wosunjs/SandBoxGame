// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource/SDResourceObject.h"
#include "Data/SDDataHandle.h"
#include "Data/SDTypes.h"
#include "Flob/SDFlobObject.h"

// Sets default values
ASDResourceObject::ASDResourceObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 实例化根节点
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	// 实例化模型组件
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("ResourceProfile"));

	// 开启交互检测
	BaseMesh->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void ASDResourceObject::BeginPlay()
{
	Super::BeginPlay();

	ResourceAttr = *SDDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	HP = BaseHP = ResourceAttr->HP;
	

}

void ASDResourceObject::CreateFlobObject()
{
	// 遍历生成掉落物
	for (TArray<TArray<int>>::TIterator It(ResourceAttr->FlobObjectInfo); It; ++It) {
		// 随机生成的数量
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		// 生成数量
		int Num = Stream.RandRange((*It)[1], (*It)[2]);

		if (GetWorld()) {
			for (int i = 0; i < Num; i++) {
				// 生成掉落物
				ASDFlobObject* FlobObject = GetWorld()->SpawnActor<ASDFlobObject>(GetActorLocation()
					+ FVector(0.f, 0.f, 20.f), FRotator::ZeroRotator);
				FlobObject->CreateFlobObject((*It)[0]);
			}
		}
	}
}

// Called every frame
void ASDResourceObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText ASDResourceObject::GetInfoText() const
{
	switch (SDDataHandle::Get()->GetLocalizationCulture())
	{
	case ECultureTeam::EN:
		return ResourceAttr->EN;
	case ECultureTeam::ZH:
		return ResourceAttr->ZH;
	}
	return ResourceAttr->EN;
}

EResourceType::Type ASDResourceObject::GetResourceType()
{
	return ResourceAttr->ResourceType;
}

float ASDResourceObject::GetHPRange()
{
	return FMath::Clamp<float>((float)HP / (float)BaseHP, 0.f, 1.f);
}

ASDResourceObject* ASDResourceObject::TakeObjectDamage(int Damage)
{
	HP = FMath::Clamp<int>(HP - Damage, 0, BaseHP);
	if (HP <= 0) {
		// 销毁
		GetWorld()->DestroyActor(this);
		//产生掉落物
		CreateFlobObject();
	}
	return this;
}

