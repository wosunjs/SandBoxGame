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
    // �����ʼ�����˲������ɹ���ֱ�ӷ���ʧ��
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;
    // �������˶���
    float ActionDuration = SECharacter->PlayHurtAction();
    // ���õȴ�ʱ��
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, ActionDuration);
    // ����¼�ί��
    FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USDEnemyTaskHurt::OnAnimationTimerDone);
    // ע�ᵽ�¼�������
    SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, ActionDuration, false);
    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USDEnemyTaskHurt::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // ���δ��ʼ����ر�����ֱ�ӷ���
    if (!InitEnemyhElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;
    // ж��ʱ��ί��
    SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    // ����
    return EBTNodeResult::Aborted;
}

void USDEnemyTaskHurt::OnAnimationTimerDone()
{
    // ��֪�������ܻ����
    SEController->FinishStateHurt();
}
