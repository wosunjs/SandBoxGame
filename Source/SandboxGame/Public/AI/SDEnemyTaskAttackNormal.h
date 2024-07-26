// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskAttackNormal.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskAttackNormal : public USDEnemyTaskBase
{
	GENERATED_BODY()

	// ��дִ��������
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector WaitTime;
	
};
