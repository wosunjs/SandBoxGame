// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/SDTypes.h"

#include "SDEnemyCharacter.generated.h"

class UChildActorComponent;
class UWidgetComponent;
class UPawnSensingComponent;
class ASDEnemyController;
class SSDEnemyHPWidget;
class USDEnemyAnim;
class UAnimationAsset;

UCLASS()
class SANDBOXGAME_API ASDEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASDEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ʵʱ����Ѫ������ʹ֮һֱ������ң���Controller����
	void UpdateHPBarRotation(FVector SPCameraLocation);

	// �޸��ƶ��ٶ�
	void SetMaxSpeed(float Speed);

	// ��ȡIdle�ȴ�ʱ��
	float GetIdleWaitTime();

	// ���Ź������������ع���ʱ��
	float PlayAttackAction(EEnemyAttakType AttackType);

	// �ܵ������������˺��ж�
	void UnderAttack(int DamageVal);

	// �������˶��������ؽ�ֱʱ��
	float PlayHurtAction();

	// ��������
	void StartDefence();

	// ֹͣ����
	void StopDefence();

	// �������ٺ���
	void DestroyEvent();

	// ��ȡ��Ʒ��Ϣ
	FText GetInfoText() const;

	// �޸��ֳ���Ʒ����Ƿ���
	void ChangeWeaponDetect(bool IsOpen);

	// ��ȡ�Ƿ��Ѿ����������
	bool IsLockPlayer();

public:
	// ��ԴID
	int ResourceIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ���ɵ�����
	void CreateFlobObject();

protected:
	// �������
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UChildActorComponent* WeaponSocket;

	// ���Ʋ��
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UChildActorComponent* SheildSocket;

	// Ѫ��
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UWidgetComponent* HPBar;

	// ���˸�֪
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UPawnSensingComponent* EnemySense;

private:
	// �󶨵��˸�֪�ķ���
	UFUNCTION()
	void OnSeePlayer(APawn* PlayerChar);
	
	// Ѫ��UI����
	TSharedPtr<SSDEnemyHPWidget> HPBarWidget;

	// ����������
	ASDEnemyController* SEController;

	// Ѫ��
	float HP;
	float MaxHP = 200.f;

	// ��ǰ��������
	USDEnemyAnim* SEAnim;

	// ����������Դ
	UAnimationAsset* AnimDead_I;
	UAnimationAsset* AnimDead_II;

	// ����ʱ��ί��
	FTimerHandle DeadHandle;
};
