// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskWander.generated.h"

struct FBlackboardKeySelector;


/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskWander : public USDEnemyTaskBase
{
	GENERATED_BODY()
	
	// 重写执行任务函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 目的地
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector Destination;

	// 等待时长
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector WaitTime;

};
