// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskLocaSP.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USDEnemyTaskLocaSP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // �����ʼ��ʧ�ܣ��򷵻�ʧ��
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

    // ��Χ��20����
    const float ChaseRadius = 20.f;

    // ��ȡ��ҵ�����֮��ĵ�λ����
    FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
    SPToSE.Normalize();

    // ����Ŀ�ĵ�Ϊ��Ҹ���
    const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;

    // �������λ��
    FVector DesLoc;

    // ʹ�õ���ϵͳ��ȡ�����
    UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);

    // �޸�Ŀ�ĵ�
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);

    // �޸��ٶ�Ϊ300
    SECharacter->SetMaxSpeed(300.f);
    
    return EBTNodeResult::Succeeded;
}
