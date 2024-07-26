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

	// 设置Idle模式，返回动作时长
	float SetIdleType(int NewType);

	// 播放攻击动画，返回动画时长
	float PlayAttackAction(EEnemyAttakType AttackType);

	// 播放受伤动画，返回动画时长
	float PlayHurtAction();

	// 停止所有动画
	void StopAllAction();

	// 开启和关闭交互动作检测碰撞
	UFUNCTION(BlueprintCallable, Category = "EnemyAnim")
	void ChangeDetection(bool IsOpen);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	float IDleType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	FVector RootBonePos;		// 根骨骼位置

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	float RootBoneAlpha;		// 根骨骼权值

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAni")
	bool IsDefence;				// 是否进入防御

protected:
	// 角色
	ASDEnemyCharacter* SECharacter;

	// 等待动作序列指针
	UAnimSequence* AnimIdle_I;
	UAnimSequence* AnimIdle_II;
	UAnimSequence* AnimIdle_III;

	// 攻击动画蒙太奇指针
	UAnimMontage* AnimAttack_I;
	UAnimMontage* AnimAttack_II;
	UAnimMontage* AnimAttack_III;
	UAnimMontage* AnimAttack_IV;

	// 攻击动画序列指针
	UAnimSequence* AnimAttackSeq_III;
	UAnimSequence* AnimAttackSeq_IV;

	// 受伤动画指针
	UAnimMontage* AnimHurt;

	// 动作计时器
	float CurrentPlayTime;
	// 动作第一帧Y轴位置
	float StartYPos;

};
