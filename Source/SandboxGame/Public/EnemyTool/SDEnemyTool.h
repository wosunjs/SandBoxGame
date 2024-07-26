// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDEnemyTool.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;

UCLASS()
class SANDBOXGAME_API ASDEnemyTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDEnemyTool();

	// �Ƿ�������
	void ChangeOverlayDetect(bool IsOpen);

	// ��������/���ƵĹ���
	static TSubclassOf<AActor> SpawnEnemyWeapon();
	static TSubclassOf<AActor> SpawnEnemySheild();

protected:
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


};
