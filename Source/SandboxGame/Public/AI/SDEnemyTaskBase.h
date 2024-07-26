// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SDEnemyTaskBase.generated.h"

class ASDEnemyController;
class ASDEnemyCharacter;

/**
 * 更新AI的动作、行为
 */
UCLASS()
class SANDBOXGAME_API USDEnemyTaskBase : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	// 初始化控制器和角色，失败返回false
	bool InitEnemyhElement(UBehaviorTreeComponent& OwnerComp);

protected:
	ASDEnemyController* SEController;

	ASDEnemyCharacter* SECharacter;
};
