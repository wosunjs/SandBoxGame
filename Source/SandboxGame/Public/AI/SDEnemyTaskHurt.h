// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SDEnemyTaskBase.h"
#include "SDEnemyTaskHurt.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskHurt : public USDEnemyTaskBase
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
	// �ڰ��ȡ�Ĺ������ͺ͵���״̬
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector WaitTime;

	// ���������������ί��
	FTimerHandle TimerHandle;
};
