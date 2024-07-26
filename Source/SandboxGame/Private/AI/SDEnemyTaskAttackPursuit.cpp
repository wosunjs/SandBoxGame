// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskAttackPursuit.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

EBTNodeResult::Type USDEnemyTaskAttackPursuit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 初始化敌人参数
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// 播放追击动画
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttakType::EA_Pursuit);
	// 设置速度为600
	SECharacter->SetMaxSpeed(600.f);
	// 设置参数
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsActionFinish.SelectedKeyName, false);
	// 添加事件委托
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USDEnemyTaskAttackPursuit::OnAnimationTimerDone);
	// 注册到事件管理器
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);

	return EBTNodeResult::Succeeded;

}

EBTNodeResult::Type USDEnemyTaskAttackPursuit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 如果初始化敌人参数不成功or事件句柄未激活，直接返回
	if (!InitEnemyhElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;

	// 卸载时间委托
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
	
}

void USDEnemyTaskAttackPursuit::OnAnimationTimerDone()
{
	// 设置动作完成
	if (SEController) SEController->ResetProcess(true);
	// 动作结束后重置速度
	if (SECharacter) SECharacter->SetMaxSpeed(300.f);
}
