// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDFirstPerPlayerAnim.h"

USDFirstPerPlayerAnim::USDFirstPerPlayerAnim()
{
	// 绑定蒙太奇资源
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerHitMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerHitMontage.FirstPlayerHitMontage'"));
	PlayerHitMontage = PlayerHitMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerEatMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerEatMontage.FirstPlayerEatMontage'"));
	PlayerEatMontage = PlayerEatMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerFightMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerFightMontage.FirstPlayerFightMontage'"));
	PlayerFightMontage = PlayerFightMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPunchMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerPunchMontage.FirstPlayerPunchMontage'"));
	PlayerPunchMontage = PlayerPunchMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPickUpMon(TEXT(
		"AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerPickUpMontage.FirstPlayerPickUpMontage'"));
	PlayerPickUpMontage = PlayerPickUpMon.Object;

	// 绑定当前视角(防止同时播放第三人称和第一人称动画)
	GameView = EGameViewMode::First;
}
