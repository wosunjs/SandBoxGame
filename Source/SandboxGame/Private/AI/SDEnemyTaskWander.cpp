// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskWander.h"
#include "Enemy/SDEnemyCharacter.h"
#include "Enemy/SDEnemyController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type USDEnemyTaskWander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 如果初始化失败，则返回失败
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

    // 设置巡逻范围
    const float WanderRadius = 1000.f;
    // 设置初始点为起点
    const FVector WanderOrigin = SECharacter->GetActorLocation();
    // 保存随机的位置
    FVector DesLoc;
    // 使用导航系统获取随机点
    UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, WanderOrigin, DesLoc, WanderRadius);
    // 当距离小于500时，重新寻找位置
    while (FVector::Distance(WanderOrigin, DesLoc) < 500.f) {
        UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, WanderOrigin, DesLoc, WanderRadius);
    }
    // 修改速度
    SECharacter->SetMaxSpeed(100.f);
    // 修改黑板上的目的地
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
    // 获取停顿时长
    float TotalWaitTime = SECharacter->GetIdleWaitTime();
    // 获取等待时长
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, TotalWaitTime);
    // 返回任务成功
    return EBTNodeResult::Succeeded;

}
