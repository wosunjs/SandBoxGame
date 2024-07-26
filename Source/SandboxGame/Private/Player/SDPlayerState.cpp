// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDPlayerState.h"
#include "Data/SDDataHandle.h"

#include "Player/SDPlayerController.h"

#include "Kismet/GameplayStatics.h"


ASDPlayerState::ASDPlayerState()
{
	// ����ÿ֡����
	PrimaryActorTick.bCanEverTick = true;

	// ��ʼ����ǰѡ�еĿ�������
	CurrentShortcutIndex = 0;

	// Ѫ���ͼ���ֵ��ʼ��
	HP = MAXHP;
	Hunger = 300.f;

	IsDead = false;
}

void ASDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// ��ӿ�����ָ������
	SPController = Cast<ASDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ASDPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// ���������Ϊ0��������Ѫ
	if (Hunger <= 0) {
		HP -= DeltaSeconds;
	}
	// ���������Ϊ0��ÿ���1������
	else {
		Hunger -= DeltaSeconds;
		// ������Ѫ
		HP += DeltaSeconds;
	}

	// �趨��Χ
	HP = FMath::Clamp<float>(HP, 0.f, MAXHP);
	Hunger = FMath::Clamp<float>(Hunger, 0.f, 300.f);

	// ִ���޸����״̬��ί��
	UpdateStateWidget.ExecuteIfBound(HP / MAXHP, Hunger / 200.f);

	// ���ѪֵΪ0��δ������ִ������
	if (HP == 0 && !IsDead) {
		IsDead = true;
	}

}

void ASDPlayerState::RegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* ContainerList, TSharedPtr<STextBlock> ShortcutInfoTextBlock)
{
	for (TArray<TSharedPtr<ShortcutContainer>>::TIterator It(*ContainerList); It; ++It) {
		ShortcutContainerList.Add(*It);
	}

	// �󶨺���
	ShortcutInfoTextAttr.BindUObject(this, &ASDPlayerState::GetShortcutInfoText);

	// �󶨿������ϢTextBlokc
	ShortcutInfoTextBlock->SetText(ShortcutInfoTextAttr);

	// ��Ʒ������
	ShortcutContainerList[0]->SetObject(7)->SetObjectNum(1);
	ShortcutContainerList[1]->SetObject(3)->SetObjectNum(3);
	/*ShortcutContainerList[3]->SetObject(3)->SetObjectNum(1);
	ShortcutContainerList[4]->SetObject(4)->SetObjectNum(35);
	ShortcutContainerList[5]->SetObject(5)->SetObjectNum(45);
	ShortcutContainerList[6]->SetObject(6)->SetObjectNum(55);
	ShortcutContainerList[7]->SetObject(7)->SetObjectNum(65);*/
	
}

void ASDPlayerState::ChooseShortcut(bool IsUp)
{
	// ��ȡ��һ����ѡ��������±�
	int NextIndex = 0;
	if (IsUp) {
		NextIndex = CurrentShortcutIndex == 0 ? 8 : CurrentShortcutIndex - 1;
	}
	else {
		NextIndex = CurrentShortcutIndex == 8 ? 0 : CurrentShortcutIndex + 1;
	}
	// ��������ѡ��
	ShortcutContainerList[CurrentShortcutIndex]->SetChoosed(false);
	ShortcutContainerList[NextIndex]->SetChoosed(true);

	// ���µ�ǰѡ�������Index
	CurrentShortcutIndex = NextIndex;
}

int ASDPlayerState::GetCurrentHandObjectIndex() const
{
	if (ShortcutContainerList.Num() == 0) return 0;
	return ShortcutContainerList[CurrentShortcutIndex]->ObjectIndex;
}

EObjectType::Type ASDPlayerState::GetCurrentObjectType()
{
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());
	return ObjectAttr->ObjectType;
}

void ASDPlayerState::RegisterRayInfoEvent(TSharedPtr<STextBlock> RayInfoTextBlock)
{
	RayInfoTextAttr.BindUObject(this, &ASDPlayerState::GetRayInfoText);

	// �����߼����Ϣ
	RayInfoTextBlock->SetText(RayInfoTextAttr);
}

int ASDPlayerState::GetAffectRange()
{
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());
	return ObjectAttr->AffectRange;
}

int ASDPlayerState::GetDamageValue(EResourceType::Type ResourceType)
{
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());
	switch (ResourceType)
	{
	case EResourceType::Plant:
		return ObjectAttr->PlantAttack;
		break;
	case EResourceType::Metal:
		return ObjectAttr->MetalAttack;
		break;
	case EResourceType::Animal:
		return ObjectAttr->AnimalAttack;
		break;
	default:
		break;
	}
	return ObjectAttr->PlantAttack;
}

void ASDPlayerState::ChangeHandObject(int ShortcutID, int ObjectID, int ObjectNum)
{
	// ���Ŀ������Ϣ
	ShortcutContainerList[ShortcutID]->SetObject(ObjectID)->SetObjectNum(ObjectNum);

	// �����ǰ�����ѡ�У������ֳ�
	SPController->ChangeHandObject();
}

void ASDPlayerState::PromoteHunger()
{
	if (Hunger > 200) {
		Hunger = 300.f;
		return;
	}
	Hunger = FMath::Clamp<float>(Hunger + 50.f, 0, 300.f);
}

bool ASDPlayerState::IsPlayerDead()
{
	return HP <= 0.f;
}

void ASDPlayerState::TakeDamage(int DamageVal)
{
	HP = FMath::Clamp<float>(HP - DamageVal, 0.f, MAXHP);
	UpdateStateWidget.ExecuteIfBound(HP / MAXHP, Hunger / 300.f);

	// ���ѪֵΪ0��δ������ִ������
	if (HP == 0 && !IsDead) {
		IsDead = true;
	}
}

// ��ȡ�������Ʒ��Ӧ����
FText ASDPlayerState::GetShortcutInfoText() const
{
	// ��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());

	// ���ݵ�ǰ���Է���������Ϣ
	switch (SDDataHandle::Get()->GetLocalizationCulture()) {
		case ECultureTeam::ZH:
			return ObjectAttr->ZH;
		case ECultureTeam::EN:
			return ObjectAttr->EN;
	}

	return FText::FromString("null");
}

FText ASDPlayerState::GetRayInfoText() const
{
	return RayInfoText;
}
