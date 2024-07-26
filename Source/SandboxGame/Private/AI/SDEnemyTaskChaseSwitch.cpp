// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskChaseSwitch.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/SDTypes.h"

EBTNodeResult::Type USDEnemyTaskChaseSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // �����ʼ��ʧ�ܣ��򷵻�ʧ��
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;

    // ��ȡ����ҵľ���
    float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());

    // �������С��300.f�򹥻�
    if (EPDistance < 300.f) {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Attack);
        return EBTNodeResult::Failed;
    }

    // �������1500��Ŀ�궪ʧ������Ѳ��״̬
    if (EPDistance > 1500.f) {
        // ���߿�������Ҷ�ʧ
        SEController->LoosePlayer();
        // ״̬�޸�ΪѲ��
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
        // ����׷��״̬
        return EBTNodeResult::Failed;
    }

    // ������һֱ׷��Ŀ��
    return EBTNodeResult::Succeeded;
}
