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
	
	// ��дִ��������
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// Ŀ�ĵ�
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector Destination;

	// �ȴ�ʱ��
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector WaitTime;

};
