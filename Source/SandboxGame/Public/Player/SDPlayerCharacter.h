// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/SDTypes.h"

#include "SDPlayerCharacter.generated.h"

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UChildActorComponent;
class ASDPlayerController;

UCLASS()
class SANDBOXGAME_API ASDPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASDPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// �޸��ӽ�
	void ChangeView(EGameViewMode::Type NewGameView);

	// �޸ĵ�ǰ�ֳ���Ʒ
	void ChangeHandObject(TSubclassOf<class AActor> HandObjectClass);

	// �޸��ֳ���Ʒ����ײ����Ƿ���
	void ChangeHandObjectDetect(bool IsOpen);

	// �Ƿ���Ⱦ�ֳ���Ʒ
	void RenderHandObject(bool IsRender);

	// ������Ʒ
	void PlayerThrowObject(int ObjectID, int Num);

	// ��鱳���Ƿ��пռ�
	bool IsPackageFree(int ObjectID);

	// �����Ʒ������
	void AddPackageObject(int ObjectID);

	// �Զ��������¼�,��Anim����
	void EatUpEvent();

	// ��ȡ��ɫĿǰ���
	FVector GetCameraLocation();

	// ��ȡ�Ƿ��Ѿ�����
	bool IsPlayerDead();

	// �ܵ��˺�
	void TakeDamage(int DamageVal);

public:
	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	UCameraComponent* FirstCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	UCameraComponent* ThirdCamera;

	UPROPERTY(BlueprintReadWrite, Category = "SD")
	float RunSpeed = 375.f;		// �����ٶ�

	// ��ҿ�����ָ��
	ASDPlayerController* SPController;

	// ��ǰ���ӽ�״̬
	EGameViewMode::Type GameView;

	// �ϰ�����״̬
	EUpperBody::Type UpperType;

	// �Ƿ������л��ӽ�
	bool IsAllowSwitchView;
	
	// �Ƿ���ס����
	bool IsInputLocked;

	// �Ƿ��ڶ�ʱ���ڽ��й�����
	bool IsAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUpAtRate(float value);
	void Turn(float value);
	void TurnAtRate(float value);
	void OnStartJump();
	void OnStopJump();
	void OnStartRun();
	void OnStopRun();

private:
	// ��һ�˳ƵĹ���ģ��
	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	USkeletalMeshComponent* MeshFirst;

	// ������Ʒ
	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	UChildActorComponent* HandObject;

	// ��ת�ٶ�(����)
	float BaseLookUpRate;
	float BaseTurnRate;


};
