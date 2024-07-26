// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskLocalESC.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type USDEnemyTaskLocalESC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 如果初始化敌人参数不成功，直接返回失败
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// 设置范围为2000
	const float ChaseRadius = 2000.f;
	// 获取敌人到玩家间的向量
	FVector SPToSE = SECharacter->GetActorLocation() - SEController->GetPlayerLocation();
	// 探索起点为当前角色位置
	const FVector ChaseOrigin = SECharacter->GetActorLocation();
	// 保存随机的位置
	FVector DesLoc(0.f);
	// 使用NAV生成随机点
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);

	// 获取撤退路径方向不是朝向玩家方向的撤退点
	while (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SPToSE, DesLoc - SECharacter->GetActorLocation()))) > 90.f) {
		// 使用导航系统随机选点
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
	}

	// 修改速度为300
	SECharacter->SetMaxSpeed(300.f);
	// 修改目的地
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	// 返回成功
	return EBTNodeResult::Succeeded;
}
