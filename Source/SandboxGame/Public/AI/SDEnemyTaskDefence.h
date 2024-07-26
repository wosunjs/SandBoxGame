// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskDefence.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskDefence : public USDEnemyTaskBase
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
	// 黑板获取的防御状态是否完成以及玩家指针
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector IsDefenceFinish;
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector PlayerPawn;

	// 攻击动作结束后的委托
	FTimerHandle TimerHandle;
};
