// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/SDTypes.h"
#include "SDPlayerController.generated.h"

class ASDPlayerCharacter;
class ASDPlayerState;

// �޸�׼��ί��
DECLARE_DELEGATE_TwoParams(FUpdatePointer, bool, float)

// ��ʾUIί��
DECLARE_DELEGATE_TwoParams(FShowGameUI, EGameUIType::Type, EGameUIType::Type)

// �޸�С��ͼ��Ұ��Χί��
DECLARE_DELEGATE_OneParam(FUpdateMiniMapWidth, int)

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASDPlayerController();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	// ��Character���ֳ���Ʒ���и��ģ����������Playerstate�ڻ����
	void ChangeHandObject();

public:
	// ��ȡ��ҽ�ɫ
	ASDPlayerCharacter* SPCharacter;

	// ��ȡ��ҿ�����
	ASDPlayerState* SPState;

	// ʵʱ�޸�׼�ǵ�ί��,ע��ĺ�����PointerWidget��UpdatePointer
	FUpdatePointer UpdatePointer;

	// ��ʾ��ϷUI����ί��,�󶨵ķ�����GameHUDWidget��ShowGameUI 
	FShowGameUI ShowGameUI;

	// ΪС��ͼ����ί����������,ע��ĺ�����ASDSceneCapture2D��UpdateMiniMapWidth
	FUpdateMiniMapWidth UpdateMiniMapWidth;

protected:
	virtual void BeginPlay() override;

private:
	// �л��ӽ�
	void ChangeView();

	// ��갴���¼�
	void LeftEventStart();
	void LeftEventStop();
	void RightEventStart();
	void RightEventStop();

	// ��껬�ֹ��¼�
	void ScrollUpEvent();
	void ScrollDownEvent();

	// �޸�Ԥ����
	void ChangePreUpperType(EUpperBody::Type RightType);
	
	// ���߼����
	FHitResult RayGetHitResult(FVector TraceStart, FVector TraceEnd);

	// ���߻���
	void DrawRayLine(FVector StartPos, FVector EndPos, float Duration);

	// �������߼��
	void RunRayCast();

	// ��Ϊ״̬��
	void StateMachine();

	// Esc�����¼�
	void EscEvent();

	// E�������¼�
	void PackageEvent();

	// T���������¼�
	void ChatRoomEvent();

	// ת������ģʽ��trueΪ��Ϸģʽ��falseΪ���ģʽ
	void SwitchInputMode(bool IsGmaeOnly);

	// ������ס����
	void LockedInput(bool IsLocked);

	// С��ͼ�����¼�
	void AddMapSizeStart();
	void AddMapSizeStop();
	void ReduceMapSizeStart();
	void ReduceMapSizeStop();

	// ����С��ͼ���ŵ�Tick����������ÿ֡����ִ��
	void TickMiniMap();

private:
	// ���Ԥ����
	EUpperBody::Type LeftUpperType;

	// �Ҽ�Ԥ����
	EUpperBody::Type RightUpperType;

	// �Ƿ�ס������갴��
	bool IsLeftButtonDown;
	bool IsRightButtonDown;

	// ��⵽����Դ
	AActor* RayActor;

	// ���浱ǰ��UI״̬
	EGameUIType::Type CurrentUIType;

	// С��ͼ����״̬
	EMiniMapSizeMode::Type MiniMapSizeMode;

};
