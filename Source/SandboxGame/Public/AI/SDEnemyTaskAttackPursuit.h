// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskAttackPursuit.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskAttackPursuit : public USDEnemyTaskBase
{
	GENERATED_BODY()

	// ��дִ��������
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// ��д������ֹ����
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// �����������¼�
	void OnAnimationTimerDone();

protected:
	// �����Ƿ���ɱ���
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector IsActionFinish;

	// ���������������ί��
	FTimerHandle TimerHandle;
};
