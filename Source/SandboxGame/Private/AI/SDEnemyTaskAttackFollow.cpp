// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskAttackFollow.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USDEnemyTaskAttackFollow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ��ʼ�����˲���
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// �������Χ5.f�����һ���λ��
	const float ChaseRadius = 0.f;
	// ��ȡ��ҵ�����֮��ĵ�λ����
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();

	// ��ȡ������ҵľ���
	float EPDistance = SPToSE.Size();

	// �����������Ҿ���Ϊ100������ͨ������Ѱ��һ����λ��
	if (EPDistance > 100.f) {
		// ����λ��
		SPToSE.Normalize();
		// ̽��Ŀ�ĵ�Ϊ�����Χ
		const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;
		// ���������λ��
		FVector DesLoc(0.f);
		// ʹ��Nav��ȡ���λ��
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
		// �޸�Ŀ�ĵ�
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	}
	// ���������100���ڣ�ֱ������Ŀ�ĵ�Ϊ����λ��
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, SECharacter->GetActorLocation());
	}
	// ����ִ��
	return EBTNodeResult::Succeeded;
}
