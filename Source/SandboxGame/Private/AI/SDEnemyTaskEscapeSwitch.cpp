// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskEscapeSwitch.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/SDTypes.h"

EBTNodeResult::Type USDEnemyTaskEscapeSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// �����ʼ�����˲������ɹ���ֱ�ӷ���ʧ��
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// ͨ���������˼�ľ��������µ�״̬
	float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
	// ����������1500�����Ѳ��
	if (EPDistance > 1500.f) {
		// ��֪��������Ҷ�ʧ
		SEController->LoosePlayer();
		// �޸�״̬ΪѲ��
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
	}
	return EBTNodeResult::Succeeded;
}
