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
	// ��AIϵͳ������֡�ʣ�0.3~0.5s�����и��£��ڰ����ݸ��¿��Է�����
	//virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
