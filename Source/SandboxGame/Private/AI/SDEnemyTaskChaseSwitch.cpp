// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskChaseSwitch.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/SDTypes.h"

EBTNodeResult::Type USDEnemyTaskChaseSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 如果初始化失败，则返回失败
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

    // 获取与玩家的距离
    float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());

    // 如果距离小于300.f则攻击
    if (EPDistance < 300.f) {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Attack);
        return EBTNodeResult::Failed;
    }

    // 如果大于1500，目标丢失，跳到巡逻状态
    if (EPDistance > 1500.f) {
        // 告诉控制器玩家丢失
        SEController->LoosePlayer();
        // 状态修改为巡逻
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
        // 跳出追逐状态
        return EBTNodeResult::Failed;
    }

    // 此外则一直追踪目标
    return EBTNodeResult::Succeeded;
}
