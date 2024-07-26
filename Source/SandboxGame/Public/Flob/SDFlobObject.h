// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDFlobObject.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTexture;
class UMaterialInstanceDynamic;
class ASDPlayerCharacter;

UCLASS()
class SANDBOXGAME_API ASDFlobObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDFlobObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ���ɵ�����Ʒ
	void CreateFlobObject(int ObjectID);

	// ������Ʒ��ʼ�������붪����ƷID�Ͷ�������
	void ThrowFlobObject(int ObjectID, float DirYaw);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// ��Ⱦ��ͼ
	void RenderTexture();

	// ��̬����¼�
	void DetectPlayer();

	// �����¼�
	void DestroyEvent();

private:
	UBoxComponent* BoxCollision;

	UStaticMeshComponent* BaseMesh;

	// ������ƷID
	int ObjectIndex;

	UTexture* ObjectIconTex;

	UMaterialInstanceDynamic* ObjectIconMatDynamic;

	// ���ָ��
	ASDPlayerCharacter* SPCharacter;

	// ��̬���Timer
	FTimerHandle DetectTimer;

	// ��ʱ����Timer
	FTimerHandle DestoryTimer;

};
