// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyTool/SDEnemyTool.h"
#include "SDEnemyWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDEnemyWeapon : public ASDEnemyTool
{
	GENERATED_BODY()

public:
	ASDEnemyWeapon();
	
protected:
	//// ÖØÐ´½»»¥¼ì²âº¯Êý
	//UFUNCTION()
	//virtual void OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
