// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDThirdPerPlayerAnim.h"
#include "Player/SDPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



USDThirdPerPlayerAnim::USDThirdPerPlayerAnim()
{
	// ����̫����Դ
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerHitMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerHitMontage.PlayerHitMontage'"));
	PlayerHitMontage = PlayerHitMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerEatMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerEatMontage.PlayerEatMontage'"));
	PlayerEatMontage = PlayerEatMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerFightMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerFightMontage.PlayerFightMontage'"));
	PlayerFightMontage = PlayerFightMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPunchMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerPunchMontage.PlayerPunchMontage'"));
	PlayerPunchMontage = PlayerPunchMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPickUpMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerPickUpMontage.PlayerPickUpMontage'"));
	PlayerPickUpMontage = PlayerPickUpMon.Object;

	// ������ֵ
	Direction = 0.f;
	// ��ǰ״̬����Ծ
	IsInAir = false;
	// �󶨵�ǰ�ӽ�(��ֹͬʱ���ŵ����˳ƺ͵�һ�˳ƶ���)
	GameView = EGameViewMode::Third;
}

void USDThirdPerPlayerAnim::UpdateParameter()
{
	// �ȵ��û���ĸ����ٶ�
	Super::UpdateParameter();

	// ������ͼָ����ɫ���
	if (!SPCharacter) {
		return;
	}

	// ��ȡ�Ƿ�����Ծ
	IsInAir = SPCharacter->GetCharacterMovement()->IsFalling();

	// ��ȡ��ת����(Yaw Ϊ-180��+180)(�ٶȳ�����(Controller) - ��ɫ������ = �ƶ���ת����)
	float PreDir = SPCharacter->GetVelocity().ToOrientationRotator().Yaw - SPCharacter->GetActorRotation().Yaw;
	if (PreDir > 180.f) {
		PreDir -= 360.f;
	}
	else if (PreDir < -180.f) {
		PreDir += 360.f;
	}
	Direction = PreDir;
}
