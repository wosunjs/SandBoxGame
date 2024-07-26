// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SDEnemyController.generated.h"

class ASDPlayerCharacter;
class ASDEnemyCharacter;
class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	ASDEnemyController();

	virtual void Tick(float DeltaTime) override;

	// ��ȡ��ҵ�λ��
	FVector GetPlayerLocation() const;

	// ��ȡ����Ƿ��Ѿ�����
	bool IsPlayerDead();

	// �������ʱ����Character��OnSeePlayer����
	void OnSeePlayer(APawn* PlayerChar);

	// ��ʧ��Ҷ�λ
	void LoosePlayer();

	// �ж�����Ƿ���Զ��
	bool IsPlayerAway();

	// ��ȡ���ָ��
	UObject* GetPlayerPawn();

	// ��֪Blackboard�����Ƿ����
	void ResetProcess(bool IsFinish);

	// �����˺�������ʣ������ֵ
	void UpdateDamageRatio(float HPRatioVal);

	// ����ܻ�״̬
	void FinishStateHurt();

	// ��ɷ���״̬
	void FinishDefenceState();

	// ��������
	void EnemyDead();

public:
	// �Ƿ����������
	bool IsLockPlayer;

protected:
	virtual void BeginPlay() override;

	// ��дOnPossess��OnUnPossess���������͹ر���Ϊ��
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	// �������״̬����
	void UpdateStateParam();

private:
	// ���ָ��
	ASDPlayerCharacter* SPCharacter;
	
	// ���˽�ɫָ��
	ASDEnemyCharacter* SECharacter;

	// �ڰ����
	UBlackboardComponent* BlackboardComp;

	// ��Ϊ�����
	UBehaviorTreeComponent* BehaviorTreeComp;

	// ��������Ҽ�ľ���,ÿ�����������
	TArray<float> EPDisList;

	// ʱ��ί�о��
	FTimerHandle EPDisHandle;

	// Ѫ��ʣ��ٷֱ�
	float HPRatio;

	// �Ƿ�����ת������״̬
	bool IsAllowHurt;

	// ���˼�ʱ��
	float HurtTimerCount;

};
