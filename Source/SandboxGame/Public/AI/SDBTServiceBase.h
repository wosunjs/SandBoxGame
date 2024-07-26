// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SDBTServiceBase.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDBTServiceBase : public UBTService
{
	GENERATED_BODY()
	
protected:
	// 按AI系统的特殊帧率（0.3~0.5s）进行更新，黑板数据更新可以放这里
	//virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
