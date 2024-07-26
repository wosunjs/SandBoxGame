// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskAttackNormal.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USDEnemyTaskAttackNormal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 初始化敌人参数
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

    // 播放普通攻击动画
    float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttakType::EA_Normal);

    // 设置参数
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, AttackDuration);

    // 返回成功
    return EBTNodeResult::Succeeded;
}
