// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDResourceObject.generated.h"

UCLASS()
class SANDBOXGAME_API ASDResourceObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDResourceObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ��ȡ��Ʒ��Ϣ
	FText GetInfoText() const;

	// ��ȡ��Դ����
	EResourceType::Type GetResourceType();

	// ��ȡѪ���ٷֱ�
	float GetHPRange();

	// ��ȡ�˺�
	ASDResourceObject* TakeObjectDamage(int Damage);

public:
	// ��ԴID
	int ResourceIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ���ɵ�����
	void CreateFlobObject();

protected:
	// �����
	USceneComponent* RootScene;

	// ��̬ģ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SD")
	UStaticMeshComponent* BaseMesh;

	// ������Դ��ַ����������洢��Դ
	TArray<FString> ResourcePath;

	// ���浱ǰ��Դ������Ϣ
	TSharedPtr<ResourceAttribute> ResourceAttr;

	// Ѫ��
	int HP;

	// ����Ѫ��
	int BaseHP;

};
