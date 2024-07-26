// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/SDTypes.h"

#include "SDPlayerState.generated.h"

// 更新玩家UI状态委托
DECLARE_DELEGATE_TwoParams(FUpdateStateWidget, float, float)

class ASDPlayerController;

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASDPlayerState();

	virtual void Tick(float DeltaSeconds) override;

	// 提供给ShortcutWidget的添加快捷栏容器委托
	void RegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* ContainerList, TSharedPtr<STextBlock> ShortcutInfoTextBlock);

	// 切换快捷栏
	void ChooseShortcut(bool IsUp);

	// 获取当前被选中容器中的物品所在Map中的Index
	int GetCurrentHandObjectIndex() const;

	// 获取当前被选中容器中的物品的类型
	EObjectType::Type GetCurrentObjectType();

	// 提供给RayInfoWidget的注册射线信息事件
	void RegisterRayInfoEvent(TSharedPtr<STextBlock> RayInfoTextBlock);
	
	// 获取当前攻击范围
	int GetAffectRange();

	// 根据类型获取造成伤害
	int GetDamageValue(EResourceType::Type ResourceType);

	// 更改快捷栏信息
	void ChangeHandObject(int ShortcutID, int ObjectID, int ObjectNum);

	// 提升饥饿值
	void PromoteHunger();

	// 获取玩家是否死亡
	bool IsPlayerDead();

	// 处理伤害
	void TakeDamage(int DamageVal);

protected:
	virtual void BeginPlay() override;

public:
	// 当前被选中的快捷栏序号
	int CurrentShortcutIndex;

	// 射线检测信息Text,由PlayerController进行更新
	FText RayInfoText;

	// 更新玩家状态UI，绑定的方法是PlayerStateWidget的UpdateStateWidget
	FUpdateStateWidget UpdateStateWidget;

	// 获取控制器指针
	ASDPlayerController* SPController;

private:
	// 获取快捷栏物品信息
	FText GetShortcutInfoText() const;

	// 获取射线检测信息
	FText GetRayInfoText() const;



private:
	// 快捷栏序列
	TArray<TSharedPtr<ShortcutContainer>> ShortcutContainerList;

	// 快捷栏信息参数(TAttribute为保存了值和获取该值委托的结构)
	TAttribute<FText> ShortcutInfoTextAttr;

	// 射线信息参数
	TAttribute<FText> RayInfoTextAttr;

	// 血量和饥饿度
	float HP;
	float MAXHP = 200.f;
	float Hunger;

	// 是否死亡
	bool IsDead;

};
