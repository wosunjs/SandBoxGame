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

	// ʵ�������ڵ�
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	// ʵ����ģ�����
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("ResourceProfile"));

	// �����������
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
	// �������ɵ�����
	for (TArray<TArray<int>>::TIterator It(ResourceAttr->FlobObjectInfo); It; ++It) {
		// ������ɵ�����
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		// ��������
		int Num = Stream.RandRange((*It)[1], (*It)[2]);

		if (GetWorld()) {
			for (int i = 0; i < Num; i++) {
				// ���ɵ�����
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
		// ����
		GetWorld()->DestroyActor(this);
		//����������
		CreateFlobObject();
	}
	return this;
}

