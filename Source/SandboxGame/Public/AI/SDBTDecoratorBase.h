// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SDBTDecoratorBase.generated.h"

/**
 * �жϽڵ��Ƿ������������
 */
UCLASS()
class SANDBOXGAME_API USDBTDecoratorBase : public UBTDecorator
{
	GENERATED_BODY()
	
	// ��Ӧ������ͼ�ڵ��PerformConditionCheck
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
