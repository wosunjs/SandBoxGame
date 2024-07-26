// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/SDTypes.h"

#include "SDEnemyCharacter.generated.h"

class UChildActorComponent;
class UWidgetComponent;
class UPawnSensingComponent;
class ASDEnemyController;
class SSDEnemyHPWidget;
class USDEnemyAnim;
class UAnimationAsset;

UCLASS()
class SANDBOXGAME_API ASDEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASDEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 实时更新血条朝向，使之一直面向玩家，有Controller调用
	void UpdateHPBarRotation(FVector SPCameraLocation);

	// 修改移动速度
	void SetMaxSpeed(float Speed);

	// 获取Idle等待时长
	float GetIdleWaitTime();

	// 播放攻击动画，返回攻击时长
	float PlayAttackAction(EEnemyAttakType AttackType);

	// 受到攻击，进行伤害判定
	void UnderAttack(int DamageVal);

	// 播放受伤动画，返回僵直时长
	float PlayHurtAction();

	// 开启防御
	void StartDefence();

	// 停止防御
	void StopDefence();

	// 死亡销毁函数
	void DestroyEvent();

	// 获取物品信息
	FText GetInfoText() const;

	// 修改手持物品检测是否开启
	void ChangeWeaponDetect(bool IsOpen);

	// 获取是否已经锁定了玩家
	bool IsLockPlayer();

public:
	// 资源ID
	int ResourceIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 生成掉落物
	void CreateFlobObject();

protected:
	// 武器插槽
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UChildActorComponent* WeaponSocket;

	// 盾牌插槽
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UChildActorComponent* SheildSocket;

	// 血条
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UWidgetComponent* HPBar;

	// 敌人感知
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UPawnSensingComponent* EnemySense;

private:
	// 绑定敌人感知的方法
	UFUNCTION()
	void OnSeePlayer(APawn* PlayerChar);
	
	// 血条UI引用
	TSharedPtr<SSDEnemyHPWidget> HPBarWidget;

	// 控制器引用
	ASDEnemyController* SEController;

	// 血量
	float HP;
	float MaxHP = 200.f;

	// 当前动作引用
	USDEnemyAnim* SEAnim;

	// 死亡动画资源
	UAnimationAsset* AnimDead_I;
	UAnimationAsset* AnimDead_II;

	// 死亡时间委托
	FTimerHandle DeadHandle;
};
