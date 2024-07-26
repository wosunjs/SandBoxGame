// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hand/SDHandObjcet.h"
#include "SDHandSword.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDHandSword : public ASDHandObjcet
{
	GENERATED_BODY()

public:
	ASDHandSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
