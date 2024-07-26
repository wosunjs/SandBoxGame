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
	// 通过重写PostLoad()方法添加参数到黑板(如果在构造函数中添加参数则会导致类型消失)
	virtual void PostLoad() override;

};
