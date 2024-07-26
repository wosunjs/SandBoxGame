// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskLocalESC.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskLocalESC : public USDEnemyTaskBase
{
	GENERATED_BODY()
	
	// 重写执行任务函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 黑板获取的玩家与敌人间距离
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector Destination;
};
