// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskDefence.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

EBTNodeResult::Type USDEnemyTaskDefence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 如果初始化敌人参数不成功，直接返回失败
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;
	// 告知Character进入防御状态，播放防御动画
	SECharacter->StartDefence();
	// 设置黑板值————没有结束防御状态
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsDefenceFinish.SelectedKeyName, false);
	// 设置黑板上的玩家指针
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerPawn.SelectedKeyName, SEController->GetPlayerPawn());
	// 添加事件委托
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USDEnemyTaskDefence::OnAnimationTimerDone);
	// 注册到事件管理器
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.f, false);	// 防御时间为2s
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USDEnemyTaskDefence::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 如果未初始化相关变量，直接返回
	if (!InitEnemyhElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;
	// 卸载时间委托
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	// 返回
	return EBTNodeResult::Aborted;
}

void USDEnemyTaskDefence::OnAnimationTimerDone()
{
	// 告诉控制器防御完成
	if(SEController) SEController->FinishDefenceState();
}
