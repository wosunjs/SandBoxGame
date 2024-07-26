// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hand/SDHandObjcet.h"
#include "SDHandHammer.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDHandHammer : public ASDHandObjcet
{
	GENERATED_BODY()
	
public:
	ASDHandHammer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ExtendMesh;

};
