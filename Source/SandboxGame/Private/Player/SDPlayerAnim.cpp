// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDPlayerAnim.h"
#include "Player/SDPlayerCharacter.h"

USDPlayerAnim::USDPlayerAnim()
{
	// 初始化
	Speed = 0.f;
	SpineRotator = FRotator(0.f, 0.f, 0.f);
}

// 将初始化角色指针和更新数据分离，方便重写子类更新蓝图数据
void USDPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	// 初始化角色指针
	InitSpCharacter();
	// 更新蓝图数据
	UpdateParameter();
	// 播放蒙太奇动画
	UpdateMontage();
}

void USDPlayerAnim::ChangeDetection(bool IsOppen)
{
	if (!SPCharacter) return;
	SPCharacter->ChangeHandObjectDetect(IsOppen);
}

void USDPlayerAnim::RenderHandObject(bool IsRender)
{
	if (!SPCharacter) return;
	SPCharacter->RenderHandObject(IsRender);
}

void USDPlayerAnim::EatUpEvent()
{
	if (!SPCharacter) return;
	SPCharacter->EatUpEvent();
}

void USDPlayerAnim::InitSpCharacter()
{
	if (!SPCharacter) {
		SPCharacter = Cast<ASDPlayerCharacter>(TryGetPawnOwner());
	}
}

void USDPlayerAnim::UpdateParameter()
{
	// 指针非空
	if (!SPCharacter) return;

	// 设置属性值
	Speed = SPCharacter->GetVelocity().Size();		// 获取速度

	// 设置上半身旋转,使值位于[-180, 180]且转换为蓝图中的xyz
	float spineDir = SPCharacter->GetActorRotation().Yaw - 90.f;
	if (spineDir > 180.f) {
		spineDir -= 360.f;
	}
	else if (spineDir < -180.f) {
		spineDir += 360.f;
	}
	SpineRotator = FRotator(0.f, spineDir, 90.f);


}

void USDPlayerAnim::UpdateMontage()
{
	if (!SPCharacter) {
		return;
	}

	// 如果发生了人称视角切换，则不会立刻更新动作
	if (SPCharacter->GameView != GameView) {
		return;
	}

	// 如果当前动作未播放完则不更新动作
	if (!Montage_GetIsStopped(CurrentMontage)) {
		return;
	}

	switch (SPCharacter->UpperType)
	{
	case EUpperBody::None:
		if (CurrentMontage != nullptr) {
			Montage_Stop(0);	// 停止所有动作
			CurrentMontage = nullptr;
			// 动画播放结束后解锁视角切换
			AllowViewChange(true);
		}
		break;
	case EUpperBody::Punch:
		if (!Montage_IsPlaying(PlayerPunchMontage)) {
			Montage_Play(PlayerPunchMontage);
			CurrentMontage = PlayerPunchMontage;
			// 动画播放完之前不允许切换视角
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Hit:
		if (!Montage_IsPlaying(PlayerHitMontage)) {
			Montage_Play(PlayerHitMontage);
			CurrentMontage = PlayerHitMontage;
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Fight:
		if (!Montage_IsPlaying(PlayerFightMontage)) {
			Montage_Play(PlayerFightMontage);
			CurrentMontage = PlayerFightMontage;
			AllowViewChange(false);
		}
		break;
	case EUpperBody::PickUp:
		if (!Montage_IsPlaying(PlayerPickUpMontage)) {
			Montage_Play(PlayerPickUpMontage);
			CurrentMontage = PlayerPickUpMontage;
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Eat:
		if (!Montage_IsPlaying(PlayerEatMontage)) {
			Montage_Play(PlayerEatMontage);
			CurrentMontage = PlayerEatMontage;
			AllowViewChange(false);
		}
		break;
	}
}

void USDPlayerAnim::AllowViewChange(bool IsAllow)
{
	if (!SPCharacter) {
		return;
	}

	SPCharacter->IsAllowSwitchView = IsAllow;
}
