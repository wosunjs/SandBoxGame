// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDPlayerState.h"
#include "Data/SDDataHandle.h"

#include "Player/SDPlayerController.h"

#include "Kismet/GameplayStatics.h"


ASDPlayerState::ASDPlayerState()
{
	// 允许每帧运行
	PrimaryActorTick.bCanEverTick = true;

	// 初始化当前选中的快捷栏序号
	CurrentShortcutIndex = 0;

	// 血量和饥饿值初始化
	HP = MAXHP;
	Hunger = 300.f;

	IsDead = false;
}

void ASDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// 添加控制器指针引用
	SPController = Cast<ASDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ASDPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 如果饥饿度为0，持续减血
	if (Hunger <= 0) {
		HP -= DeltaSeconds;
	}
	// 如果饥饿不为0，每秒掉1饥饿度
	else {
		Hunger -= DeltaSeconds;
		// 持续回血
		HP += DeltaSeconds;
	}

	// 设定范围
	HP = FMath::Clamp<float>(HP, 0.f, MAXHP);
	Hunger = FMath::Clamp<float>(Hunger, 0.f, 300.f);

	// 执行修改玩家状态的委托
	UpdateStateWidget.ExecuteIfBound(HP / MAXHP, Hunger / 200.f);

	// 如果血值为0且未死亡，执行死亡
	if (HP == 0 && !IsDead) {
		IsDead = true;
	}

}

void ASDPlayerState::RegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* ContainerList, TSharedPtr<STextBlock> ShortcutInfoTextBlock)
{
	for (TArray<TSharedPtr<ShortcutContainer>>::TIterator It(*ContainerList); It; ++It) {
		ShortcutContainerList.Add(*It);
	}

	// 绑定函数
	ShortcutInfoTextAttr.BindUObject(this, &ASDPlayerState::GetShortcutInfoText);

	// 绑定快捷栏信息TextBlokc
	ShortcutInfoTextBlock->SetText(ShortcutInfoTextAttr);

	// 物品栏测试
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
	// 获取下一个被选择的容器下标
	int NextIndex = 0;
	if (IsUp) {
		NextIndex = CurrentShortcutIndex == 0 ? 8 : CurrentShortcutIndex - 1;
	}
	else {
		NextIndex = CurrentShortcutIndex == 8 ? 0 : CurrentShortcutIndex + 1;
	}
	// 设置容器选中
	ShortcutContainerList[CurrentShortcutIndex]->SetChoosed(false);
	ShortcutContainerList[NextIndex]->SetChoosed(true);

	// 更新当前选择的容器Index
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

	// 绑定射线检测信息
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
	// 更改快捷栏信息
	ShortcutContainerList[ShortcutID]->SetObject(ObjectID)->SetObjectNum(ObjectNum);

	// 如果当前快捷栏选中，更新手持
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

	// 如果血值为0且未死亡，执行死亡
	if (HP == 0 && !IsDead) {
		IsDead = true;
	}
}

// 获取快捷栏物品对应名字
FText ASDPlayerState::GetShortcutInfoText() const
{
	// 获取物品属性
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());

	// 根据当前语言返回名字信息
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
