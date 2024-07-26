// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "SDEnemyBlackboard.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyBlackboard : public UBlackboardData
{
	GENERATED_BODY()
	
public:
	// ͨ����дPostLoad()������Ӳ������ڰ�(����ڹ��캯������Ӳ�����ᵼ��������ʧ)
	virtual void PostLoad() override;

};
