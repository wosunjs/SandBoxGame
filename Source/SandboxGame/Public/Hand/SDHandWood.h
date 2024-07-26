// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hand/SDHandObjcet.h"
#include "SDHandWood.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDHandWood : public ASDHandObjcet
{
	GENERATED_BODY()

public:
	ASDHandWood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
