// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskLocaSP.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USDEnemyTaskLocaSP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 如果初始化失败，则返回失败
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

    // 范围是20以内
    const float ChaseRadius = 20.f;

    // 获取玩家到敌人之间的单位向量
    FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
    SPToSE.Normalize();

    // 设置目的地为玩家附近
    const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;

    // 保存随机位置
    FVector DesLoc;

    // 使用导航系统获取随机点
    UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);

    // 修改目的地
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);

    // 修改速度为300
    SECharacter->SetMaxSpeed(300.f);
    
    return EBTNodeResult::Succeeded;
}
