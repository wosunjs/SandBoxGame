// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDPlayerController.h"
#include "Player/SDPlayerCharacter.h"
#include "Data/SDTypes.h"
#include "Player/SDPlayerState.h"
#include "Hand/SDHandObjcet.h"
#include "Components/LineBatchComponent.h"
#include "Camera/CameraComponent.h"
#include "Pickup/SDPickupObject.h"
#include "Resource/SDResourceObject.h"
#include "Enemy/SDEnemyCharacter.h"

ASDPlayerController::ASDPlayerController()
{
	// ����ÿ֡������
	PrimaryActorTick.bCanEverTick = true;
}

void ASDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// ��ȡ��ɫ
	if (!SPCharacter) {
		SPCharacter = Cast<ASDPlayerCharacter>(GetCharacter());
	}

	// ��ȡ���״̬
	if (!SPState) {
		SPState = Cast<ASDPlayerState>(PlayerState);
	}

	// ������겻��ʾ
	bShowMouseCursor = false;

	// ��������ģʽ
	FInputModeGameOnly InputMode;

	// ��������
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);

	// ����Ԥ������ֵ
	LeftUpperType = EUpperBody::Punch;
	RightUpperType = EUpperBody::PickUp;

	// ��ʼ�����δ����
	IsLeftButtonDown = false;
	IsRightButtonDown = false;

	// ��ʼ��UIΪ��Ϸ״̬
	CurrentUIType = EGameUIType::Game;

	// ��ʼ��С��ͼ����ģʽ
	MiniMapSizeMode = EMiniMapSizeMode::None;

}


void ASDPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// �������߼��
	RunRayCast();

	// ������״̬
	StateMachine();

	// С��ͼ����
	TickMiniMap();
}

void ASDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// ���ӽ��л�
	InputComponent->BindAction("ChangeView", IE_Pressed, this, &ASDPlayerController::ChangeView);

	// ����갴���¼�
	InputComponent->BindAction("LeftEvent", IE_Pressed, this, &ASDPlayerController::LeftEventStart);
	InputComponent->BindAction("LeftEvent", IE_Released, this, &ASDPlayerController::LeftEventStop);
	InputComponent->BindAction("RightEvent", IE_Pressed, this, &ASDPlayerController::RightEventStart);
	InputComponent->BindAction("RightEvent", IE_Released, this, &ASDPlayerController::RightEventStop);

	// ���������¼�
	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &ASDPlayerController::ScrollUpEvent);
	InputComponent->BindAction("ScrollDown", IE_Pressed, this, &ASDPlayerController::ScrollDownEvent);

	// ��ESC�¼������õ���ͣ��Ϸ��ʱ����Ȼ��������
	InputComponent->BindAction("EscEvent", IE_Pressed, this, &ASDPlayerController::EscEvent)
		.bExecuteWhenPaused = true;		// ��δ���ã�����Ϸ��ͣʱ�޷��������¼�

	// ��B����
	InputComponent->BindAction("PackageEvent", IE_Pressed, this, &ASDPlayerController::PackageEvent);

	// ��T����
	InputComponent->BindAction("ChatRoomEvent", IE_Pressed, this, &ASDPlayerController::ChatRoomEvent);

	// ��С��ͼ�Ŵ���С
	InputComponent->BindAction("AddMapSize", IE_Pressed, this, &ASDPlayerController::AddMapSizeStart);
	InputComponent->BindAction("AddMapSize", IE_Released, this, &ASDPlayerController::AddMapSizeStop);
	InputComponent->BindAction("ReduceMapSize", IE_Pressed, this, &ASDPlayerController::ReduceMapSizeStart);
	InputComponent->BindAction("ReduceMapSize", IE_Released, this, &ASDPlayerController::ReduceMapSizeStop);

}

void ASDPlayerController::ChangeHandObject()
{
	// �����ֳ���Ʒ
	SPCharacter->ChangeHandObject(ASDHandObjcet::SpawnHandObject(SPState->GetCurrentHandObjectIndex()));
}

void ASDPlayerController::ChangeView()
{
	if (SPCharacter->IsInputLocked) {
		return;
	}

	// ����������л��ӽ�ֱ�ӷ���
	if (!SPCharacter->IsAllowSwitchView) {
		return;
	}

	// �л��������ģ��
	switch (SPCharacter->GameView) 
	{
	case EGameViewMode::First:
		SPCharacter->ChangeView(EGameViewMode::Third);
		break;
	case EGameViewMode::Third:
		SPCharacter->ChangeView(EGameViewMode::First);
		break;
	}
}

void ASDPlayerController::LeftEventStart()
{
	// ����Ƿ��������
	if (SPCharacter->IsInputLocked) {
		return;
	}

	SPCharacter->UpperType = LeftUpperType;
	IsLeftButtonDown = true;
}

void ASDPlayerController::LeftEventStop()
{
	if (SPCharacter->IsInputLocked) {
		return;
	}

	SPCharacter->UpperType = EUpperBody::None;
	IsLeftButtonDown = false;
}

void ASDPlayerController::RightEventStart()
{
	if (SPCharacter->IsInputLocked) {
		return;
	}

	SPCharacter->UpperType = RightUpperType;
	IsRightButtonDown = true;
}

void ASDPlayerController::RightEventStop()
{
	if (SPCharacter->IsInputLocked) {
		return;
	}

	SPCharacter->UpperType = EUpperBody::None;
	IsRightButtonDown = false;
}

void ASDPlayerController::ScrollUpEvent()
{
	if (SPCharacter->IsInputLocked) {
		return;
	}

	// ����ӽǲ������л���ֱ�ӷ���
	if (!SPCharacter->IsAllowSwitchView) return;

	// ���������Ҽ������������л�
	if (IsLeftButtonDown || IsRightButtonDown) return;

	// ��PlayerState�л�����
	SPState->ChooseShortcut(true);
	ChangeHandObject();
}

void ASDPlayerController::ScrollDownEvent()
{
	if (SPCharacter->IsInputLocked) {
		return;
	}

	// ����������л���ֱ�ӷ���
	if (!SPCharacter->IsAllowSwitchView) return;

	// ���������Ҽ������������л�
	if (IsLeftButtonDown || IsRightButtonDown) return;

	// ��PlayerState�л�����
	SPState->ChooseShortcut(false);
	ChangeHandObject();
}

void ASDPlayerController::EscEvent()
{
	switch (CurrentUIType) {
	case EGameUIType::Game:
		// ��Ϸ״̬�°���ESC����
		SetPause(true);
		// ����ģʽ����ΪGameAndUI
		SwitchInputMode(false);
		// ������Ϸ����
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Pause);
		// ���µ�ǰUIType
		CurrentUIType = EGameUIType::Pause;
		// ����ҶԽ�ɫ�Ŀ�����������
		LockedInput(true);
		break;
	case EGameUIType::Pause:
	case EGameUIType::Package:
	case EGameUIType::ChatRoom:
		// ����״̬�°���ESC������Ϸ
		SetPause(false);
		// ��������ģʽΪGameOnly
		SwitchInputMode(true);
		// ������Ϸ����
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Game);
		// ���µ�ǰUI
		CurrentUIType = EGameUIType::Game;
		// ����ҶԽ�ɫ�Ŀ����������
		LockedInput(false);
		break;
	}
}

void ASDPlayerController::PackageEvent()
{
	switch (CurrentUIType)
	{
	case EGameUIType::Game:
		// ��������ģʽ
		SwitchInputMode(false);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Package);
		CurrentUIType = EGameUIType::Package;
		LockedInput(true);
		break;
	case EGameUIType::Package:
		SwitchInputMode(true);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Game);
		CurrentUIType = EGameUIType::Game;
		LockedInput(false);
		break;
	}
}

void ASDPlayerController::ChatRoomEvent()
{
	switch (CurrentUIType)
	{
	case EGameUIType::Game:
		// ��������ģʽ
		SwitchInputMode(false);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::ChatRoom);
		CurrentUIType = EGameUIType::ChatRoom;
		LockedInput(true);
		break;
	case EGameUIType::ChatRoom:
		SwitchInputMode(true);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Game);
		CurrentUIType = EGameUIType::Game;
		LockedInput(false);
		break;
	}
}

void ASDPlayerController::SwitchInputMode(bool IsGmaeOnly)
{
	if (IsGmaeOnly) {
		// �������
		bShowMouseCursor = false;
		// ��������ģʽΪOnlyGame
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(true);	// ���ư����¼��ύ��Controller
		SetInputMode(InputMode);
	}
	else {
		// ��ʾ���
		bShowMouseCursor = true;
		// ��������ģʽΪGameAndUI
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);	// ¼��ʱ���������
		SetInputMode(InputMode);
	}
}

void ASDPlayerController::LockedInput(bool IsLocked)
{
	SPCharacter->IsInputLocked = IsLocked;
}

void ASDPlayerController::ChangePreUpperType(EUpperBody::Type RightType = EUpperBody::None)
{
	// ���ݵ�ǰ�ֳ���Ʒ��������Ԥ����
	switch (SPState->GetCurrentObjectType()) {
	case EObjectType::Normal:
		LeftUpperType = EUpperBody::Punch;
		RightUpperType = RightType;
		break;
	case EObjectType::Food:
		LeftUpperType = EUpperBody::Punch;
		// ����Ҽ�״̬��ʰȡ�Ǿ͸�ʰȡ(ʰȡ���������ȼ���)
		RightUpperType = RightType == EUpperBody::None ? EUpperBody::Eat : RightType;
		break;
	case EObjectType::Tool:
		LeftUpperType = EUpperBody::Hit;
		RightUpperType = RightType;
		break;
	case EObjectType::Weapon:
		LeftUpperType = EUpperBody::Fight;
		RightUpperType = RightType;
		break;
	}

}

FHitResult ASDPlayerController::RayGetHitResult(FVector TraceStart, FVector TraceEnd)
{
	FCollisionQueryParams TraceParams(true);
	TraceParams.AddIgnoredActor(SPCharacter);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;
	
	FHitResult Hit(ForceInit);
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1,
		TraceParams)) {
		//DrawRayLine(TraceStart, TraceEnd, 5.f);
	}

	return Hit;
}

void ASDPlayerController::DrawRayLine(FVector StartPos, FVector EndPos, float Duration)
{
	ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
	if (LineBatcher != nullptr) {
		float LineDurationTime = Duration > 0.f ? Duration : LineBatcher->DefaultLifeTime;
		LineBatcher->DrawLine(StartPos, EndPos, FLinearColor::Red, 10, 0.f, LineDurationTime);
	}
}

void ASDPlayerController::RunRayCast()
{
	FVector StartPos(0.f);
	FVector EndPos(0.f);

	switch (SPCharacter->GameView)
	{
	case EGameViewMode::First:
		StartPos = SPCharacter->FirstCamera->K2_GetComponentLocation();
		EndPos = StartPos + SPCharacter->FirstCamera->GetForwardVector() * 2000.f;
		break;
	case EGameViewMode::Third:
		StartPos = SPCharacter->ThirdCamera->K2_GetComponentLocation();
		StartPos = StartPos + SPCharacter->ThirdCamera->GetForwardVector() * 300.f;
		EndPos = StartPos + SPCharacter->ThirdCamera->GetForwardVector() * 2000.f;
		break;
	}

	// �Ƿ��⵽��Ʒ
	bool IsDetected = false;
	FHitResult Hit = RayGetHitResult(StartPos, EndPos);
	RayActor = Hit.GetActor();

	if (Cast<ASDPickupObject>(RayActor)) {
		IsDetected = true;
		SPState->RayInfoText = Cast<ASDPickupObject>(RayActor)->GetInfoText();
	}
	else if (Cast<ASDResourceObject>(RayActor)) {
		IsDetected = true;
		SPState->RayInfoText = Cast<ASDResourceObject>(RayActor)->GetInfoText();
	}
	else if (Cast<ASDEnemyCharacter>(RayActor)) {
		IsDetected = true;
		SPState->RayInfoText = Cast<ASDEnemyCharacter>(RayActor)->GetInfoText();
	}
	else {
		SPState->RayInfoText = FText();
	}
}

void ASDPlayerController::StateMachine()
{
	// ʵ�ֶ����޸�
	// ��ͨģʽ
	ChangePreUpperType(EUpperBody::None);
	if (!Cast<ASDResourceObject>(RayActor) && !Cast<ASDPickupObject>(RayActor) && !Cast<ASDEnemyCharacter>(RayActor)) {
		// �����׼��Ϊ����Դ��ʰȡ��׼��Ϊ��ɫ
		UpdatePointer.ExecuteIfBound(false, 1.f);
	}

	// �����⵽���ˣ�����׼��
	if (Cast<ASDEnemyCharacter>(RayActor)) {
		// ׼������ģʽ
		UpdatePointer.ExecuteIfBound(false, 0.f);
	}

	// �����⵽��Դ
	if (Cast<ASDResourceObject>(RayActor)) {
		  // ���������£�׼�Ǳ�Ϊ��ɫ��Ϊ����ģʽ
		if (!IsLeftButtonDown) {
			UpdatePointer.ExecuteIfBound(false, 0.f);
		}
		// ���������£�������
		else {
			if (FVector::Distance(RayActor->GetActorLocation(), SPCharacter->GetActorLocation()) 
				< SPState->GetAffectRange()) {
				// ��ȡʵ���˺�
				int Damage = SPState->GetDamageValue(Cast<ASDResourceObject>(RayActor)->GetResourceType());
				// ����Դ�յ��˺����һ�ȡʣ��Ѫ���ٷֱ�
				float Range = Cast<ASDResourceObject>(RayActor)->TakeObjectDamage(Damage)->GetHPRange();
				// ����׼��
				UpdatePointer.ExecuteIfBound(true, Range);
			}
		}
	}
	else if (Cast<ASDPickupObject>(RayActor) && FVector::Distance(RayActor->GetActorLocation(), SPCharacter->GetActorLocation())
		< 300.f) {
		// �ı��Ҽ�Ԥ״̬Ϊʰȡ
		ChangePreUpperType(EUpperBody::PickUp);
		// �޸�׼������ģʽ
		UpdatePointer.ExecuteIfBound(false, 0);
		// ����Ҽ�����
		if (IsRightButtonDown && SPCharacter->IsPackageFree(Cast<ASDPickupObject>(RayActor)->ObjectIndex)) {
			// ����Ʒʰȡ
			SPCharacter->AddPackageObject(Cast<ASDPickupObject>(RayActor)->TakePickup());
		}
	}



}

void ASDPlayerController::AddMapSizeStart()
{
	// ��⵱ǰ�����Ƿ���ס
	if (SPCharacter->IsInputLocked) return;
	// ��������״̬
	MiniMapSizeMode = EMiniMapSizeMode::Add;
}

void ASDPlayerController::AddMapSizeStop()
{
	// ��⵱ǰ�����Ƿ���ס
	if (SPCharacter->IsInputLocked) return;
	// ��������״̬
	MiniMapSizeMode = EMiniMapSizeMode::None;
}

void ASDPlayerController::ReduceMapSizeStart()
{
	// ��⵱ǰ�����Ƿ���ס
	if (SPCharacter->IsInputLocked) return;
	// ��������״̬
	MiniMapSizeMode = EMiniMapSizeMode::Reduce;
}

void ASDPlayerController::ReduceMapSizeStop()
{
	// ��⵱ǰ�����Ƿ���ס
	if (SPCharacter->IsInputLocked) return;
	// ��������״̬
	MiniMapSizeMode = EMiniMapSizeMode::None;
}

void ASDPlayerController::TickMiniMap() {
	switch (MiniMapSizeMode)
	{
	case EMiniMapSizeMode::Add:
		UpdateMiniMapWidth.ExecuteIfBound(-5);
		break;
	case EMiniMapSizeMode::Reduce:
		UpdateMiniMapWidth.ExecuteIfBound(5);
		break;
	}
}
