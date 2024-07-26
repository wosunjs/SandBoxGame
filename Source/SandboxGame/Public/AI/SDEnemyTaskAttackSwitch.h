// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskAttackSwitch.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskAttackSwitch : public USDEnemyTaskBase
{
	GENERATED_BODY()

protected:
	// 黑板获取的攻击类型、敌人状态、玩家指针
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector AttackType;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector EnemyState;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector PlayerPawn;
	
private:
	// 重写执行任务函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
