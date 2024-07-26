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
	// �����ʼ�����˲������ɹ���ֱ�ӷ���ʧ��
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;
	// ��֪Character�������״̬�����ŷ�������
	SECharacter->StartDefence();
	// ���úڰ�ֵ��������û�н�������״̬
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsDefenceFinish.SelectedKeyName, false);
	// ���úڰ��ϵ����ָ��
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerPawn.SelectedKeyName, SEController->GetPlayerPawn());
	// ����¼�ί��
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USDEnemyTaskDefence::OnAnimationTimerDone);
	// ע�ᵽ�¼�������
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.f, false);	// ����ʱ��Ϊ2s
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USDEnemyTaskDefence::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ���δ��ʼ����ر�����ֱ�ӷ���
	if (!InitEnemyhElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;
	// ж��ʱ��ί��
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	// ����
	return EBTNodeResult::Aborted;
}

void USDEnemyTaskDefence::OnAnimationTimerDone()
{
	// ���߿������������
	if(SEController) SEController->FinishDefenceState();
}
