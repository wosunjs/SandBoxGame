// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hand/SDHandObjcet.h"
#include "SDHandAxe.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDHandAxe : public ASDHandObjcet
{
	GENERATED_BODY()
	
public:
	ASDHandAxe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
