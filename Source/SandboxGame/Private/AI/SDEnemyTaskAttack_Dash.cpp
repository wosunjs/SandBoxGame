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
	// ��ʼ�����˲���
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// ����ͻ������
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttakType::EA_Dash);

	// �������Χ5.f�����һ���λ��
	const float ChaseRadius = 5.f;
	// ��ȡ��ҵ�����֮��ĵ�λ����
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
	SPToSE.Normalize();
	// ̽��Ŀ�ĵ�Ϊ�����Χ
	const FVector ChaseOrigin = SEController->GetPlayerLocation() - 20.f * SPToSE;
	// ���������λ��
	FVector DesLoc(0.f);
	// ʹ��Nav��ȡ���λ��
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);

	// ��ȡ��̬�ٶ���ɳ��
	float Speed = (FVector::Distance(SECharacter->GetActorLocation(), DesLoc)) / AttackDuration + 30.f;
	// �޸��ٶ�
	SECharacter->SetMaxSpeed(Speed);

	// �޸�Ŀ�ĵ�
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	// ���õȴ�ʱ��
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, AttackDuration);

	// ����¼�ί��
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USDEnemyTaskAttack_Dash::OnAnimationTimerDone);
	// ע�ᵽ�¼�������
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);
	
	// ����ִ��
	return EBTNodeResult::Succeeded;

}

EBTNodeResult::Type USDEnemyTaskAttack_Dash::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// �����ʼ�����˲������ɹ�or�¼����δ���ֱ�ӷ���
	if (!InitEnemyhElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;

	// ж��ʱ��ί��
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USDEnemyTaskAttack_Dash::OnAnimationTimerDone()
{
	// ���������������ٶ�
	if (SECharacter) SECharacter->SetMaxSpeed(300.f);
}
