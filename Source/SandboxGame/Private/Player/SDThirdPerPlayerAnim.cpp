// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDThirdPerPlayerAnim.h"
#include "Player/SDPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



USDThirdPerPlayerAnim::USDThirdPerPlayerAnim()
{
	// 绑定蒙太奇资源
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

	// 给方向赋值
	Direction = 0.f;
	// 当前状态非跳跃
	IsInAir = false;
	// 绑定当前视角(防止同时播放第三人称和第一人称动画)
	GameView = EGameViewMode::Third;
}

void USDThirdPerPlayerAnim::UpdateParameter()
{
	// 先调用基类的更新速度
	Super::UpdateParameter();

	// 动画蓝图指定角色检查
	if (!SPCharacter) {
		return;
	}

	// 获取是否在跳跃
	IsInAir = SPCharacter->GetCharacterMovement()->IsFalling();

	// 获取旋转方向(Yaw 为-180到+180)(速度朝向方向(Controller) - 角色朝向方向 = 移动旋转方向)
	float PreDir = SPCharacter->GetVelocity().ToOrientationRotator().Yaw - SPCharacter->GetActorRotation().Yaw;
	if (PreDir > 180.f) {
		PreDir -= 360.f;
	}
	else if (PreDir < -180.f) {
		PreDir += 360.f;
	}
	Direction = PreDir;
}
