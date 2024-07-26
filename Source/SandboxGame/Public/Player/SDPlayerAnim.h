// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include  "Data/SDTypes.h"

#include "SDPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	USDPlayerAnim();

	// ��ֵ��ʾ��Ӧ����
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	// �����͹ر�������Ʒ�Ľ������
	UFUNCTION(BlueprintCallable, Category = "SD")
	void ChangeDetection(bool IsOppen);

	// �����͹ر�������Ʒ����ʾ�����ʰȡʱ����
	UFUNCTION(BlueprintCallable, Category = "PlayAnim")
	void RenderHandObject(bool IsRender);

	// ����ʱ����
	UFUNCTION(BlueprintCallable, Category = "PlayAnim")
	void EatUpEvent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayAnim")
	float Speed;	// �����е��ٶ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayAnim")
	FRotator SpineRotator;

protected:
	// ��ȡ��ɫָ��
	void InitSpCharacter();

	// ��������
	virtual void UpdateParameter();

	// ���¶���
	virtual void UpdateMontage();

	// ���������ӽ��л���
	void AllowViewChange(bool IsAllow);

protected:
	// ʹ�ö�����ͼ�Ľ�ɫָ��
	class ASDPlayerCharacter* SPCharacter;

	// �ϰ����Montage
	UAnimMontage* PlayerHitMontage;
	UAnimMontage* PlayerFightMontage;
	UAnimMontage* PlayerPunchMontage;
	UAnimMontage* PlayerEatMontage;
	UAnimMontage* PlayerPickUpMontage;

	// ���浱ǰ���ŵ�Montage
	UAnimMontage* CurrentMontage;

	// ָ���Լ��������˳�
	EGameViewMode::Type GameView;


};
