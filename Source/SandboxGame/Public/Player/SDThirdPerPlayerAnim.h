// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SDPlayerAnim.h"
#include "SDThirdPerPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USDThirdPerPlayerAnim : public USDPlayerAnim
{
	GENERATED_BODY()
	
public:
	USDThirdPerPlayerAnim();

protected:
	// 重写更新属性方法
	virtual void UpdateParameter() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayAnim")
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayAnim")
	float Direction;
};
