// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskAttackFollow.generated.h"

/**
 * ʵʱ��ȡ���λ�ò����׷��
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskAttackFollow : public USDEnemyTaskBase
{
	GENERATED_BODY()

	// ��дִ��������
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector Destination;
	
};
