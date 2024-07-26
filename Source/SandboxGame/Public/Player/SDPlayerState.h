// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/SDTypes.h"

#include "SDPlayerState.generated.h"

// �������UI״̬ί��
DECLARE_DELEGATE_TwoParams(FUpdateStateWidget, float, float)

class ASDPlayerController;

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASDPlayerState();

	virtual void Tick(float DeltaSeconds) override;

	// �ṩ��ShortcutWidget����ӿ��������ί��
	void RegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* ContainerList, TSharedPtr<STextBlock> ShortcutInfoTextBlock);

	// �л������
	void ChooseShortcut(bool IsUp);

	// ��ȡ��ǰ��ѡ�������е���Ʒ����Map�е�Index
	int GetCurrentHandObjectIndex() const;

	// ��ȡ��ǰ��ѡ�������е���Ʒ������
	EObjectType::Type GetCurrentObjectType();

	// �ṩ��RayInfoWidget��ע��������Ϣ�¼�
	void RegisterRayInfoEvent(TSharedPtr<STextBlock> RayInfoTextBlock);
	
	// ��ȡ��ǰ������Χ
	int GetAffectRange();

	// �������ͻ�ȡ����˺�
	int GetDamageValue(EResourceType::Type ResourceType);

	// ���Ŀ������Ϣ
	void ChangeHandObject(int ShortcutID, int ObjectID, int ObjectNum);

	// ��������ֵ
	void PromoteHunger();

	// ��ȡ����Ƿ�����
	bool IsPlayerDead();

	// �����˺�
	void TakeDamage(int DamageVal);

protected:
	virtual void BeginPlay() override;

public:
	// ��ǰ��ѡ�еĿ�������
	int CurrentShortcutIndex;

	// ���߼����ϢText,��PlayerController���и���
	FText RayInfoText;

	// �������״̬UI���󶨵ķ�����PlayerStateWidget��UpdateStateWidget
	FUpdateStateWidget UpdateStateWidget;

	// ��ȡ������ָ��
	ASDPlayerController* SPController;

private:
	// ��ȡ�������Ʒ��Ϣ
	FText GetShortcutInfoText() const;

	// ��ȡ���߼����Ϣ
	FText GetRayInfoText() const;



private:
	// ���������
	TArray<TSharedPtr<ShortcutContainer>> ShortcutContainerList;

	// �������Ϣ����(TAttributeΪ������ֵ�ͻ�ȡ��ֵί�еĽṹ)
	TAttribute<FText> ShortcutInfoTextAttr;

	// ������Ϣ����
	TAttribute<FText> RayInfoTextAttr;

	// Ѫ���ͼ�����
	float HP;
	float MAXHP = 200.f;
	float Hunger;

	// �Ƿ�����
	bool IsDead;

};
