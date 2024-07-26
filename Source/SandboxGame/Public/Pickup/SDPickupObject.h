// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDPickupObject.generated.h"

UCLASS()
class SANDBOXGAME_API ASDPickupObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDPickupObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ��ȡ��Ʒ��Ϣ
	FText GetInfoText() const;

	// ��Ʒ��ʰȡ,������ƷID
	int TakePickup();


public:
	int ObjectIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// �����
	USceneComponent* RootScene;

	// ��̬ģ��
	UStaticMeshComponent* BaseMesh;
	

};
