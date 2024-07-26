// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SDBTDecoratorBase.generated.h"

/**
 * 判断节点是否接着往下运行
 */
UCLASS()
class SANDBOXGAME_API USDBTDecoratorBase : public UBTDecorator
{
	GENERATED_BODY()
	
	// 对应的是蓝图节点的PerformConditionCheck
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
