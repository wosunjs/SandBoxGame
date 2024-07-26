// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskAttackSwitch.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/SDTypes.h"

#include "common/SDHelper.h"

EBTNodeResult::Type USDEnemyTaskAttackSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 初始化敌人参数
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;
  
    // 如果玩家已经死亡
    if (SEController->IsPlayerDead()) {
        // 告诉控制器玩家丢失
        SEController->LoosePlayer();
        // 修改状态为巡逻
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
        // 跳出攻击状态
        return EBTNodeResult::Failed;
    }

    // 黑板获取玩家指针
    OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerPawn.SelectedKeyName, SEController->GetPlayerPawn());

    // 获取与玩家的距离
    float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
    SDHelper::Debug("Distance Between E & P is " + FString::FromInt(EPDistance), 0.f);

    // 如果距离小于200，修改攻击状态为普攻
    if (EPDistance < 200.f) {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttakType::EA_Normal);
        return EBTNodeResult::Succeeded;
    }
    // 如果距离小于300
    else if (EPDistance < 300.f) {
        // 判断玩家在远离，则追逐攻击
        if (SEController->IsPlayerAway()) {
            OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttakType::EA_Pursuit);
            return EBTNodeResult::Succeeded;
        }
        // 否则冲刺攻击
        else {
            OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttakType::EA_Dash);
            return EBTNodeResult::Succeeded;
        }
    }
    // 如果距离大于300,修改攻击状态为继续追逐
    else {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Chase);
        // 跳出攻击状态
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Failed;
}
