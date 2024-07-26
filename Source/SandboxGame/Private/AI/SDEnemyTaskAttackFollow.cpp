// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskAttackFollow.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USDEnemyTaskAttackFollow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 初始化敌人参数
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// 在玩家周围5.f处随机一冲击位置
	const float ChaseRadius = 0.f;
	// 获取玩家到敌人之间的单位向量
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();

	// 获取距离玩家的距离
	float EPDistance = SPToSE.Size();

	// 如果敌人与玩家距离为100以上则通过导航寻找一合适位置
	if (EPDistance > 100.f) {
		// 方向单位化
		SPToSE.Normalize();
		// 探索目的地为玩家周围
		const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;
		// 保存随机的位置
		FVector DesLoc(0.f);
		// 使用Nav获取随机位置
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
		// 修改目的地
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	}
	// 如果距离在100以内，直接设置目的地为脚下位置
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, SECharacter->GetActorLocation());
	}
	// 继续执行
	return EBTNodeResult::Succeeded;
}
