// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskAttackNormal.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USDEnemyTaskAttackNormal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // ��ʼ�����˲���
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

    // ������ͨ��������
    float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttakType::EA_Normal);

    // ���ò���
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, AttackDuration);

    // ���سɹ�
    return EBTNodeResult::Succeeded;
}
