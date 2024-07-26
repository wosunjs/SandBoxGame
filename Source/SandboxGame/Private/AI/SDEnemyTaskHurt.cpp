// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskHurt.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

EBTNodeResult::Type USDEnemyTaskHurt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 如果初始化敌人参数不成功，直接返回失败
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;
    // 播放受伤动画
    float ActionDuration = SECharacter->PlayHurtAction();
    // 设置等待时间
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, ActionDuration);
    // 添加事件委托
    FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USDEnemyTaskHurt::OnAnimationTimerDone);
    // 注册到事件管理器
    SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, ActionDuration, false);
    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USDEnemyTaskHurt::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 如果未初始化相关变量，直接返回
    if (!InitEnemyhElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;
    // 卸载时间委托
    SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    // 返回
    return EBTNodeResult::Aborted;
}

void USDEnemyTaskHurt::OnAnimationTimerDone()
{
    // 告知控制器受击完成
    SEController->FinishStateHurt();
}
