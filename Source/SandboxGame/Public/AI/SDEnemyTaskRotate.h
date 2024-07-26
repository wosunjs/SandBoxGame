// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskRotate.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskRotate : public USDEnemyTaskBase
{
	GENERATED_BODY()

	// 重写执行任务函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
