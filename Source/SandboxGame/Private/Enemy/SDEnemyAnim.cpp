// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SDEnemyAnim.h"
#include "Enemy/SDEnemyCharacter.h"
#include "Animation/AnimSequence.h"


USDEnemyAnim::USDEnemyAnim()
{
	// ��ȡ����
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_I(TEXT(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_I.Enemy_Idle_I'"));
	AnimIdle_I = StaticAnimIdle_I.Object;
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_II(TEXT(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_II.Enemy_Idle_II'"));
	AnimIdle_II = StaticAnimIdle_II.Object;
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_III(TEXT(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_III.Enemy_Idle_III'"));
	AnimIdle_III = StaticAnimIdle_III.Object;

	// ��ȡ��̫���ļ�
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimAttack_I(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Attack_I.MonEnemy_Attack_I'"));
	AnimAttack_I = StaticAnimAttack_I.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimAttack_II(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Attack_II.MonEnemy_Attack_II'"));
	AnimAttack_II = StaticAnimAttack_II.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimAttack_III(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Attack_III.MonEnemy_Attack_III'"));
	AnimAttack_III = StaticAnimAttack_III.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimAttack_IV(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Attack_IV.MonEnemy_Attack_IV'"));
	AnimAttack_IV = StaticAnimAttack_IV.Object;

	// ��ȡ������������
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimAttackSeq_III(TEXT(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Attack_III.Enemy_Attack_III'"));
	AnimAttackSeq_III = StaticAnimAttackSeq_III.Object;
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimAttackSeq_IV(TEXT(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Attack_IV.Enemy_Attack_IV'"));
	AnimAttackSeq_IV = StaticAnimAttackSeq_IV.Object;

	// ��ȡ���˶�����Դ
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimHurt(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Hurt.MonEnemy_Hurt'"));
	AnimHurt = StaticAnimHurt.Object;

	// ��ʼ������
	Speed = 0.f;
	IDleType = 0.f;
	RootBonePos = FVector::ZeroVector;
	RootBoneAlpha = 0.f;
	CurrentPlayTime = 0.f;
	StartYPos = 0.f;
	IsDefence = false;
}

void USDEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	// ��ʼ����ɫָ��
	if (!SECharacter) SECharacter = Cast<ASDEnemyCharacter>(TryGetPawnOwner());
	if (!SECharacter) return;

	// �����ٶ�
	Speed = SECharacter->GetVelocity().Size();

	// ������ڲ��Ź�����������ʵʱ�޸ĸ�����λ��,ʹ֮һֱ��ԭ����ɹ�������
	if (Montage_IsPlaying(AnimAttack_III)) {
		CurrentPlayTime += DeltaSeconds;
		// ��ȡ����ʱ��
		CurrentPlayTime = FMath::Clamp<float>(CurrentPlayTime, 0.f, AnimAttackSeq_III->GetPlayLength());
		// ��ȡ�����仯
		FTransform OutputTrans;
		AnimAttackSeq_III->GetBoneTransform(OutputTrans, 0.f, CurrentPlayTime, true);
		// ���ø�����λ��
		RootBonePos = FVector(OutputTrans.GetLocation().X, StartYPos, OutputTrans.GetLocation().Z);
	}

	// ������ڲ��Ź��������ģ�ʵʱ�޸ĸ�����λ��,ʹ֮һֱ��ԭ����ɹ�������
	if (Montage_IsPlaying(AnimAttack_IV)) {
		CurrentPlayTime += DeltaSeconds;
		// ��ȡ����ʱ��
		CurrentPlayTime = FMath::Clamp<float>(CurrentPlayTime, 0.f, AnimAttackSeq_IV->GetPlayLength());
		// ��ȡ�����仯
		FTransform OutputTrans;
		AnimAttackSeq_IV->GetBoneTransform(OutputTrans, 0.f, CurrentPlayTime, true);
		// ���ø�����λ��
		RootBonePos = FVector(OutputTrans.GetLocation().X, StartYPos, OutputTrans.GetLocation().Z);
	}
	
	// ������ٲ��Ź���λ�ƶ���������Ȩ�ش���0����0.3��������Ȩֵ
	if (!Montage_IsPlaying(AnimAttack_III) && !Montage_IsPlaying(AnimAttack_IV) && RootBoneAlpha > 0.f) {
		RootBoneAlpha -= DeltaSeconds * 3;
		if (RootBoneAlpha < 0.f) RootBoneAlpha = 0.f;
	}
}

float USDEnemyAnim::SetIdleType(int NewType)
{
	IDleType = FMath::Clamp<float>((float)NewType, 0.f, 2.f);
	switch (NewType)
	{
	case 0:
		return AnimIdle_I->GetPlayLength();
	case 1:
		return AnimIdle_II->GetPlayLength();
	case 2:
		return AnimIdle_III->GetPlayLength();
	}
	return AnimIdle_I->GetPlayLength();
}

float USDEnemyAnim::PlayAttackAction(EEnemyAttakType AttackType)
{
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int SelectIndex = Stream.RandRange(0, 1);
	FTransform OutputTrans;
	switch (AttackType)
	{
	case EEnemyAttakType::EA_Normal:
		// ѡ�����������ͨ��������
		if (SelectIndex == 0) {
			// ���ŵ�һ����������
			Montage_Play(AnimAttack_I);
			return AnimAttack_I->GetPlayLength();
		}
		else {
			Montage_Play(AnimAttack_II);
			return AnimAttack_II->GetPlayLength();
		}
		break;
	case EEnemyAttakType::EA_Dash:
		// ���Ź�����
		CurrentPlayTime = 0.f;
		RootBonePos = FVector::ZeroVector;
		RootBoneAlpha = 1.f;
		// ��ȡ������������һ֡��Yֵ
		AnimAttackSeq_III->GetBoneTransform(OutputTrans, 0, 0.f, true);
		StartYPos = OutputTrans.GetLocation().Y;
		return AnimAttack_III->GetPlayLength();
		break;
	case EEnemyAttakType::EA_Pursuit:
		// ���Ź�����
		CurrentPlayTime = 0.f;
		RootBonePos = FVector::ZeroVector;
		RootBoneAlpha = 1.f;
		// ��ȡ���������ĵ�һ֡��Yֵ
		AnimAttackSeq_IV->GetBoneTransform(OutputTrans, 0, 0.f, true);
		StartYPos = OutputTrans.GetLocation().Y;
		return AnimAttack_IV->GetPlayLength();
		break;
		break;
	}
	return 0;
}

float USDEnemyAnim::PlayHurtAction()
{
	if (!Montage_IsPlaying(AnimHurt)) Montage_Play(AnimHurt);
	return AnimHurt->GetPlayLength();
}

void USDEnemyAnim::StopAllAction()
{
	// ֹͣȫ������
	Montage_Stop(0);
}

void USDEnemyAnim::ChangeDetection(bool IsOpen)
{
	if (SECharacter) SECharacter->ChangeWeaponDetect(IsOpen);
}
