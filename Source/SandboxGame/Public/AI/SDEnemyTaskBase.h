// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SDEnemyTaskBase.generated.h"

class ASDEnemyController;
class ASDEnemyCharacter;

/**
 * ����AI�Ķ�������Ϊ
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskBase : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	// ��ʼ���������ͽ�ɫ��ʧ�ܷ���false
	bool InitEnemyhElement(UBehaviorTreeComponent& OwnerComp);

protected:
	ASDEnemyController* SEController;

	ASDEnemyCharacter* SECharacter;
};
