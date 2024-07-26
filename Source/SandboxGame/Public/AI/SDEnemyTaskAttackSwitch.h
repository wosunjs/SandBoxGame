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
	// �ڰ��ȡ�Ĺ������͡�����״̬�����ָ��
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector AttackType;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector EnemyState;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector PlayerPawn;
	
private:
	// ��дִ��������
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
