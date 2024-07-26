// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDPlayerAnim.h"
#include "Player/SDPlayerCharacter.h"

USDPlayerAnim::USDPlayerAnim()
{
	// ��ʼ��
	Speed = 0.f;
	SpineRotator = FRotator(0.f, 0.f, 0.f);
}

// ����ʼ����ɫָ��͸������ݷ��룬������д���������ͼ����
void USDPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	// ��ʼ����ɫָ��
	InitSpCharacter();
	// ������ͼ����
	UpdateParameter();
	// ������̫�涯��
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
	// ָ��ǿ�
	if (!SPCharacter) return;

	// ��������ֵ
	Speed = SPCharacter->GetVelocity().Size();		// ��ȡ�ٶ�

	// �����ϰ�����ת,ʹֵλ��[-180, 180]��ת��Ϊ��ͼ�е�xyz
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

	// ����������˳��ӽ��л����򲻻����̸��¶���
	if (SPCharacter->GameView != GameView) {
		return;
	}

	// �����ǰ����δ�������򲻸��¶���
	if (!Montage_GetIsStopped(CurrentMontage)) {
		return;
	}

	switch (SPCharacter->UpperType)
	{
	case EUpperBody::None:
		if (CurrentMontage != nullptr) {
			Montage_Stop(0);	// ֹͣ���ж���
			CurrentMontage = nullptr;
			// �������Ž���������ӽ��л�
			AllowViewChange(true);
		}
		break;
	case EUpperBody::Punch:
		if (!Montage_IsPlaying(PlayerPunchMontage)) {
			Montage_Play(PlayerPunchMontage);
			CurrentMontage = PlayerPunchMontage;
			// ����������֮ǰ�������л��ӽ�
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
