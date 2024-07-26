// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskEscapeSwitch.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/SDTypes.h"

EBTNodeResult::Type USDEnemyTaskEscapeSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 如果初始化敌人参数不成功，直接返回失败
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// 通过玩家与敌人间的距离设置新的状态
	float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
	// 如果距离大于1500则进入巡逻
	if (EPDistance > 1500.f) {
		// 告知控制器玩家丢失
		SEController->LoosePlayer();
		// 修改状态为巡逻
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
	}
	return EBTNodeResult::Succeeded;
}
