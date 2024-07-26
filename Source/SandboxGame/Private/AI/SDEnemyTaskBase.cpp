// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyTaskBase.h"
#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyCharacter.h"

bool USDEnemyTaskBase::InitEnemyhElement(UBehaviorTreeComponent& OwnerComp)
{
    // 避免重复初始化
    if (SEController && SECharacter) return true;

    // 初始化
    SEController = Cast<ASDEnemyController>(OwnerComp.GetAIOwner());
    if (SEController) {
        SECharacter = Cast<ASDEnemyCharacter>(SEController->GetPawn());
        if (SECharacter) return true;
    }

    return false;
}
