// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskLocalESC.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type USDEnemyTaskLocalESC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// �����ʼ�����˲������ɹ���ֱ�ӷ���ʧ��
	if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

	// ���÷�ΧΪ2000
	const float ChaseRadius = 2000.f;
	// ��ȡ���˵���Ҽ������
	FVector SPToSE = SECharacter->GetActorLocation() - SEController->GetPlayerLocation();
	// ̽�����Ϊ��ǰ��ɫλ��
	const FVector ChaseOrigin = SECharacter->GetActorLocation();
	// ���������λ��
	FVector DesLoc(0.f);
	// ʹ��NAV���������
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);

	// ��ȡ����·�������ǳ�����ҷ���ĳ��˵�
	while (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SPToSE, DesLoc - SECharacter->GetActorLocation()))) > 90.f) {
		// ʹ�õ���ϵͳ���ѡ��
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
	}

	// �޸��ٶ�Ϊ300
	SECharacter->SetMaxSpeed(300.f);
	// �޸�Ŀ�ĵ�
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	// ���سɹ�
	return EBTNodeResult::Succeeded;
}
