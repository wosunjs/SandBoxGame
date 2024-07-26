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

	// 赋值显示对应动作
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	// 开启和关闭手上物品的交互检测
	UFUNCTION(BlueprintCallable, Category = "SD")
	void ChangeDetection(bool IsOppen);

	// 开启和关闭手上物品的显示与否，在拾取时调用
	UFUNCTION(BlueprintCallable, Category = "PlayAnim")
	void RenderHandObject(bool IsRender);

	// 吃完时调用
	UFUNCTION(BlueprintCallable, Category = "PlayAnim")
	void EatUpEvent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayAnim")
	float Speed;	// 动画中的速度

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayAnim")
	FRotator SpineRotator;

protected:
	// 获取角色指针
	void InitSpCharacter();

	// 更新属性
	virtual void UpdateParameter();

	// 更新动作
	virtual void UpdateMontage();

	// 更新允许视角切换锁
	void AllowViewChange(bool IsAllow);

protected:
	// 使用动画蓝图的角色指针
	class ASDPlayerCharacter* SPCharacter;

	// 上半身的Montage
	UAnimMontage* PlayerHitMontage;
	UAnimMontage* PlayerFightMontage;
	UAnimMontage* PlayerPunchMontage;
	UAnimMontage* PlayerEatMontage;
	UAnimMontage* PlayerPickUpMontage;

	// 保存当前播放的Montage
	UAnimMontage* CurrentMontage;

	// 指定自己的运行人称
	EGameViewMode::Type GameView;


};
