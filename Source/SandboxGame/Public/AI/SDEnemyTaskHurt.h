// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskHurt.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskHurt : public USDEnemyTaskBase
{
	GENERATED_BODY()

	// 重写执行任务函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 重写任务终止函数
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 动作结束后事件
	void OnAnimationTimerDone();

protected:
	// 黑板获取的攻击类型和敌人状态
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector WaitTime;

	// 攻击动作结束后的委托
	FTimerHandle TimerHandle;
};
