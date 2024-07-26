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
	// ��ʼ�����˲���
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// ����׷������
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttakType::EA_Pursuit);
	// �����ٶ�Ϊ600
	SECharacter->SetMaxSpeed(600.f);
	// ���ò���
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsActionFinish.SelectedKeyName, false);
	// ����¼�ί��
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USDEnemyTaskAttackPursuit::OnAnimationTimerDone);
	// ע�ᵽ�¼�������
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);

	return EBTNodeResult::Succeeded;

}

EBTNodeResult::Type USDEnemyTaskAttackPursuit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// �����ʼ�����˲������ɹ�or�¼����δ���ֱ�ӷ���
	if (!InitEnemyhElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;

	// ж��ʱ��ί��
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
	
}

void USDEnemyTaskAttackPursuit::OnAnimationTimerDone()
{
	// ���ö������
	if (SEController) SEController->ResetProcess(true);
	// ���������������ٶ�
	if (SECharacter) SECharacter->SetMaxSpeed(300.f);
}
