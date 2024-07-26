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
	// 允许每帧都允许
	PrimaryActorTick.bCanEverTick = true;
}

void ASDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 获取角色
	if (!SPCharacter) {
		SPCharacter = Cast<ASDPlayerCharacter>(GetCharacter());
	}

	// 获取玩家状态
	if (!SPState) {
		SPState = Cast<ASDPlayerState>(PlayerState);
	}

	// 设置鼠标不显示
	bShowMouseCursor = false;

	// 设置输入模式
	FInputModeGameOnly InputMode;

	// 处理输入
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);

	// 设置预动作初值
	LeftUpperType = EUpperBody::Punch;
	RightUpperType = EUpperBody::PickUp;

	// 初始化鼠标未按下
	IsLeftButtonDown = false;
	IsRightButtonDown = false;

	// 初始化UI为游戏状态
	CurrentUIType = EGameUIType::Game;

	// 初始化小地图缩放模式
	MiniMapSizeMode = EMiniMapSizeMode::None;

}


void ASDPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 进行射线检测
	RunRayCast();

	// 处理动作状态
	StateMachine();

	// 小地图更新
	TickMiniMap();
}

void ASDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 绑定视角切换
	InputComponent->BindAction("ChangeView", IE_Pressed, this, &ASDPlayerController::ChangeView);

	// 绑定鼠标按下事件
	InputComponent->BindAction("LeftEvent", IE_Pressed, this, &ASDPlayerController::LeftEventStart);
	InputComponent->BindAction("LeftEvent", IE_Released, this, &ASDPlayerController::LeftEventStop);
	InputComponent->BindAction("RightEvent", IE_Pressed, this, &ASDPlayerController::RightEventStart);
	InputComponent->BindAction("RightEvent", IE_Released, this, &ASDPlayerController::RightEventStop);

	// 绑定鼠标滚动事件
	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &ASDPlayerController::ScrollUpEvent);
	InputComponent->BindAction("ScrollDown", IE_Pressed, this, &ASDPlayerController::ScrollDownEvent);

	// 绑定ESC事件并设置当暂停游戏的时候依然可以运行
	InputComponent->BindAction("EscEvent", IE_Pressed, this, &ASDPlayerController::EscEvent)
		.bExecuteWhenPaused = true;		// 若未设置，则当游戏暂停时无法触发该事件

	// 绑定B背包
	InputComponent->BindAction("PackageEvent", IE_Pressed, this, &ASDPlayerController::PackageEvent);

	// 绑定T聊天
	InputComponent->BindAction("ChatRoomEvent", IE_Pressed, this, &ASDPlayerController::ChatRoomEvent);

	// 绑定小地图放大缩小
	InputComponent->BindAction("AddMapSize", IE_Pressed, this, &ASDPlayerController::AddMapSizeStart);
	InputComponent->BindAction("AddMapSize", IE_Released, this, &ASDPlayerController::AddMapSizeStop);
	InputComponent->BindAction("ReduceMapSize", IE_Pressed, this, &ASDPlayerController::ReduceMapSizeStart);
	InputComponent->BindAction("ReduceMapSize", IE_Released, this, &ASDPlayerController::ReduceMapSizeStop);

}

void ASDPlayerController::ChangeHandObject()
{
	// 生成手持物品
	SPCharacter->ChangeHandObject(ASDHandObjcet::SpawnHandObject(SPState->GetCurrentHandObjectIndex()));
}

void ASDPlayerController::ChangeView()
{
	if (SPCharacter->IsInputLocked) {
		return;
	}

	// 如果不允许切换视角直接返回
	if (!SPCharacter->IsAllowSwitchView) {
		return;
	}

	// 切换摄像机和模型
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
	// 检测是否可以输入
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

	// 如果视角不允许切换，直接返回
	if (!SPCharacter->IsAllowSwitchView) return;

	// 如果鼠标左右键按下则不允许切换
	if (IsLeftButtonDown || IsRightButtonDown) return;

	// 让PlayerState切换容器
	SPState->ChooseShortcut(true);
	ChangeHandObject();
}

void ASDPlayerController::ScrollDownEvent()
{
	if (SPCharacter->IsInputLocked) {
		return;
	}

	// 如果不允许切换，直接返回
	if (!SPCharacter->IsAllowSwitchView) return;

	// 如果鼠标左右键按下则不允许切换
	if (IsLeftButtonDown || IsRightButtonDown) return;

	// 让PlayerState切换容器
	SPState->ChooseShortcut(false);
	ChangeHandObject();
}

void ASDPlayerController::EscEvent()
{
	switch (CurrentUIType) {
	case EGameUIType::Game:
		// 游戏状态下按下ESC进入
		SetPause(true);
		// 输入模式设置为GameAndUI
		SwitchInputMode(false);
		// 更新游戏界面
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Pause);
		// 更新当前UIType
		CurrentUIType = EGameUIType::Pause;
		// 将玩家对角色的控制输入锁定
		LockedInput(true);
		break;
	case EGameUIType::Pause:
	case EGameUIType::Package:
	case EGameUIType::ChatRoom:
		// 其余状态下按下ESC返回游戏
		SetPause(false);
		// 设置输入模式为GameOnly
		SwitchInputMode(true);
		// 更新游戏界面
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Game);
		// 更新当前UI
		CurrentUIType = EGameUIType::Game;
		// 将玩家对角色的控制输入解锁
		LockedInput(false);
		break;
	}
}

void ASDPlayerController::PackageEvent()
{
	switch (CurrentUIType)
	{
	case EGameUIType::Game:
		// 设置输入模式
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
		// 设置输入模式
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
		// 隐藏鼠标
		bShowMouseCursor = false;
		// 设置输入模式为OnlyGame
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(true);	// 控制按下事件提交给Controller
		SetInputMode(InputMode);
	}
	else {
		// 显示鼠标
		bShowMouseCursor = true;
		// 设置输入模式为GameAndUI
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);	// 录制时不隐藏鼠标
		SetInputMode(InputMode);
	}
}

void ASDPlayerController::LockedInput(bool IsLocked)
{
	SPCharacter->IsInputLocked = IsLocked;
}

void ASDPlayerController::ChangePreUpperType(EUpperBody::Type RightType = EUpperBody::None)
{
	// 根据当前手持物品类型设置预动作
	switch (SPState->GetCurrentObjectType()) {
	case EObjectType::Normal:
		LeftUpperType = EUpperBody::Punch;
		RightUpperType = RightType;
		break;
	case EObjectType::Food:
		LeftUpperType = EUpperBody::Punch;
		// 如果右键状态是拾取那就给拾取(拾取动作的优先级高)
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

	// 是否检测到物品
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
	// 实现动作修改
	// 普通模式
	ChangePreUpperType(EUpperBody::None);
	if (!Cast<ASDResourceObject>(RayActor) && !Cast<ASDPickupObject>(RayActor) && !Cast<ASDEnemyCharacter>(RayActor)) {
		// 如果瞄准的为非资源和拾取则准星为白色
		UpdatePointer.ExecuteIfBound(false, 1.f);
	}

	// 如果检测到敌人，锁定准星
	if (Cast<ASDEnemyCharacter>(RayActor)) {
		// 准星锁定模式
		UpdatePointer.ExecuteIfBound(false, 0.f);
	}

	// 如果检测到资源
	if (Cast<ASDResourceObject>(RayActor)) {
		  // 如果左键按下，准星变为绿色，为锁定模式
		if (!IsLeftButtonDown) {
			UpdatePointer.ExecuteIfBound(false, 0.f);
		}
		// 如果左键按下，检测距离
		else {
			if (FVector::Distance(RayActor->GetActorLocation(), SPCharacter->GetActorLocation()) 
				< SPState->GetAffectRange()) {
				// 获取实际伤害
				int Damage = SPState->GetDamageValue(Cast<ASDResourceObject>(RayActor)->GetResourceType());
				// 让资源收到伤害并且获取剩余血量百分比
				float Range = Cast<ASDResourceObject>(RayActor)->TakeObjectDamage(Damage)->GetHPRange();
				// 更新准星
				UpdatePointer.ExecuteIfBound(true, Range);
			}
		}
	}
	else if (Cast<ASDPickupObject>(RayActor) && FVector::Distance(RayActor->GetActorLocation(), SPCharacter->GetActorLocation())
		< 300.f) {
		// 改变右键预状态为拾取
		ChangePreUpperType(EUpperBody::PickUp);
		// 修改准星锁定模式
		UpdatePointer.ExecuteIfBound(false, 0);
		// 如果右键按下
		if (IsRightButtonDown && SPCharacter->IsPackageFree(Cast<ASDPickupObject>(RayActor)->ObjectIndex)) {
			// 把物品拾取
			SPCharacter->AddPackageObject(Cast<ASDPickupObject>(RayActor)->TakePickup());
		}
	}



}

void ASDPlayerController::AddMapSizeStart()
{
	// 检测当前界面是否锁住
	if (SPCharacter->IsInputLocked) return;
	// 设置缩放状态
	MiniMapSizeMode = EMiniMapSizeMode::Add;
}

void ASDPlayerController::AddMapSizeStop()
{
	// 检测当前界面是否锁住
	if (SPCharacter->IsInputLocked) return;
	// 设置缩放状态
	MiniMapSizeMode = EMiniMapSizeMode::None;
}

void ASDPlayerController::ReduceMapSizeStart()
{
	// 检测当前界面是否锁住
	if (SPCharacter->IsInputLocked) return;
	// 设置缩放状态
	MiniMapSizeMode = EMiniMapSizeMode::Reduce;
}

void ASDPlayerController::ReduceMapSizeStop()
{
	// 检测当前界面是否锁住
	if (SPCharacter->IsInputLocked) return;
	// 设置缩放状态
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
