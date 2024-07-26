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
    // �����ʼ��ʧ�ܣ��򷵻�ʧ��
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

    // ����Ѳ�߷�Χ
    const float WanderRadius = 1000.f;
    // ���ó�ʼ��Ϊ���
    const FVector WanderOrigin = SECharacter->GetActorLocation();
    // ���������λ��
    FVector DesLoc;
    // ʹ�õ���ϵͳ��ȡ�����
    UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, WanderOrigin, DesLoc, WanderRadius);
    // ������С��500ʱ������Ѱ��λ��
    while (FVector::Distance(WanderOrigin, DesLoc) < 500.f) {
        UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, WanderOrigin, DesLoc, WanderRadius);
    }
    // �޸��ٶ�
    SECharacter->SetMaxSpeed(100.f);
    // �޸ĺڰ��ϵ�Ŀ�ĵ�
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
    // ��ȡͣ��ʱ��
    float TotalWaitTime = SECharacter->GetIdleWaitTime();
    // ��ȡ�ȴ�ʱ��
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, TotalWaitTime);
    // ��������ɹ�
    return EBTNodeResult::Succeeded;

}
