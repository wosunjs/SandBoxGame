// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDHandObjcet.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;


UCLASS()
class SANDBOXGAME_API ASDHandObjcet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDHandObjcet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ������ƷID������Ʒ�ľ�̬����
	static TSubclassOf<AActor> SpawnHandObject(int ObjectID);

	// �Ƿ�������ײ���
	void ChangeOverlayDetect(bool IsOpen);

public:
	// ��ƷID
	int ObjectIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// �������ĺ���
	UFUNCTION()
	virtual void OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// �����
	USceneComponent* RootScene;

	// ��̬ģ��
	UPROPERTY(EditAnywhere, Category = "SD")
	UStaticMeshComponent* BaseMesh;

	// ��ײ��Χ��
	UPROPERTY(EditAnywhere, Category = "SD")
	UBoxComponent* AffectCollision;
	
	// ����Ʒʱ����Ч
	USoundWave* OverlaySound;
};
