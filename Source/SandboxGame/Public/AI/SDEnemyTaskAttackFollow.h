// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskAttackFollow.generated.h"

/**
 * 实时获取玩家位置并完成追随
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskAttackFollow : public USDEnemyTaskBase
{
	GENERATED_BODY()

	// 重写执行任务函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector Destination;
	
};
