// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Data/SDTypes.h"

#include "SDEnemyAnim.generated.h"

class ASDEnemyCharacter;
class UAnimSequence;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USDEnemyAnim();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// ����Idleģʽ�����ض���ʱ��
	float SetIdleType(int NewType);

	// ���Ź������������ض���ʱ��
	float PlayAttackAction(EEnemyAttakType AttackType);

	// �������˶��������ض���ʱ��
	float PlayHurtAction();

	// ֹͣ���ж���
	void StopAllAction();

	// �����͹رս������������ײ
	UFUNCTION(BlueprintCallable, Category = "EnemyAnim")
	void ChangeDetection(bool IsOpen);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	float IDleType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	FVector RootBonePos;		// ������λ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	float RootBoneAlpha;		// ������Ȩֵ

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	bool IsDefence;				// �Ƿ�������

protected:
	// ��ɫ
	ASDEnemyCharacter* SECharacter;

	// �ȴ���������ָ��
	UAnimSequence* AnimIdle_I;
	UAnimSequence* AnimIdle_II;
	UAnimSequence* AnimIdle_III;

	// ����������̫��ָ��
	UAnimMontage* AnimAttack_I;
	UAnimMontage* AnimAttack_II;
	UAnimMontage* AnimAttack_III;
	UAnimMontage* AnimAttack_IV;

	// ������������ָ��
	UAnimSequence* AnimAttackSeq_III;
	UAnimSequence* AnimAttackSeq_IV;

	// ���˶���ָ��
	UAnimMontage* AnimHurt;

	// ������ʱ��
	float CurrentPlayTime;
	// ������һ֡Y��λ��
	float StartYPos;

};
