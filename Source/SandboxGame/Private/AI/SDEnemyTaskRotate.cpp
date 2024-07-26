// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskRotate.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USDEnemyTaskRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ��ʼ�����˲���
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
