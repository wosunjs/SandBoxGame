// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskAttackSwitch.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/SDTypes.h"

#include "common/SDHelper.h"

EBTNodeResult::Type USDEnemyTaskAttackSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // ��ʼ�����˲���
    if (!InitEnemyhElement(OwnerComp)) return EBTNodeResult::Failed;
  
    // �������Ѿ�����
    if (SEController->IsPlayerDead()) {
        // ���߿�������Ҷ�ʧ
        SEController->LoosePlayer();
        // �޸�״̬ΪѲ��
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
        // ��������״̬
        return EBTNodeResult::Failed;
    }

    // �ڰ��ȡ���ָ��
    OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerPawn.SelectedKeyName, SEController->GetPlayerPawn());

    // ��ȡ����ҵľ���
    float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
    SDHelper::Debug("Distance Between E & P is " + FString::FromInt(EPDistance), 0.f);

    // �������С��200���޸Ĺ���״̬Ϊ�չ�
    if (EPDistance < 200.f) {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttakType::EA_Normal);
        return EBTNodeResult::Succeeded;
    }
    // �������С��300
    else if (EPDistance < 300.f) {
        // �ж������Զ�룬��׷�𹥻�
        if (SEController->IsPlayerAway()) {
            OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttakType::EA_Pursuit);
            return EBTNodeResult::Succeeded;
        }
        // �����̹���
        else {
            OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttakType::EA_Dash);
            return EBTNodeResult::Succeeded;
        }
    }
    // ����������300,�޸Ĺ���״̬Ϊ����׷��
    else {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Chase);
        // ��������״̬
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Failed;
}
