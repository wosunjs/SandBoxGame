// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskAttack_Dash.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

EBTNodeResult::Type USDEnemyTaskAttack_Dash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 初始化敌人参数
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// 播放突击动画
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttakType::EA_Dash);

	// 在玩家周围5.f处随机一冲击位置
	const float ChaseRadius = 5.f;
	// 获取玩家到敌人之间的单位向量
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
	SPToSE.Normalize();
	// 探索目的地为玩家周围
	const FVector ChaseOrigin = SEController->GetPlayerLocation() - 20.f * SPToSE;
	// 保存随机的位置
	FVector DesLoc(0.f);
	// 使用Nav获取随机位置
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);

	// 获取动态速度完成冲刺
	float Speed = (FVector::Distance(SECharacter->GetActorLocation(), DesLoc)) / AttackDuration + 30.f;
	// 修改速度
	SECharacter->SetMaxSpeed(Speed);

	// 修改目的地
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	// 设置等待时间
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, AttackDuration);

	// 添加事件委托
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USDEnemyTaskAttack_Dash::OnAnimationTimerDone);
	// 注册到事件管理器
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);
	
	// 继续执行
	return EBTNodeResult::Succeeded;

}

EBTNodeResult::Type USDEnemyTaskAttack_Dash::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 如果初始化敌人参数不成功or事件句柄未激活，直接返回
	if (!InitEnemyhElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;

	// 卸载时间委托
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USDEnemyTaskAttack_Dash::OnAnimationTimerDone()
{
	// 动作结束后重置速度
	if (SECharacter) SECharacter->SetMaxSpeed(300.f);
}
