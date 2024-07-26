// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SDEnemyController.generated.h"

class ASDPlayerCharacter;
class ASDEnemyCharacter;
class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	ASDEnemyController();

	virtual void Tick(float DeltaTime) override;

	// 获取玩家的位置
	FVector GetPlayerLocation() const;

	// 获取玩家是否已经死亡
	bool IsPlayerDead();

	// 看到玩家时，由Character的OnSeePlayer调用
	void OnSeePlayer(APawn* PlayerChar);

	// 丢失玩家定位
	void LoosePlayer();

	// 判断玩家是否在远离
	bool IsPlayerAway();

	// 获取玩家指针
	UObject* GetPlayerPawn();

	// 告知Blackboard动作是否完成
	void ResetProcess(bool IsFinish);

	// 处理伤害，传入剩余生命值
	void UpdateDamageRatio(float HPRatioVal);

	// 完成受击状态
	void FinishStateHurt();

	// 完成防御状态
	void FinishDefenceState();

	// 敌人死亡
	void EnemyDead();

public:
	// 是否已锁定玩家
	bool IsLockPlayer;

protected:
	virtual void BeginPlay() override;

	// 重写OnPossess和OnUnPossess函数开启和关闭行为树
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	// 更新玩家状态序列
	void UpdateStateParam();

private:
	// 玩家指针
	ASDPlayerCharacter* SPCharacter;
	
	// 敌人角色指针
	ASDEnemyCharacter* SECharacter;

	// 黑板组件
	UBlackboardComponent* BlackboardComp;

	// 行为树组件
	UBehaviorTreeComponent* BehaviorTreeComp;

	// 保存与玩家间的距离,每半秒更新依次
	TArray<float> EPDisList;

	// 时间委托句柄
	FTimerHandle EPDisHandle;

	// 血量剩余百分比
	float HPRatio;

	// 是否允许转到受伤状态
	bool IsAllowHurt;

	// 受伤计时器
	float HurtTimerCount;

};
