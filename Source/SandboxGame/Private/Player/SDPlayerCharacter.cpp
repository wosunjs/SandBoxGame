// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/SDTypes.h"
#include "Flob/SDFlobObject.h"
#include "Player/SDPackageManager.h"
#include "Player/SDPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SDPlayerState.h"

#include "Hand/SDHandObjcet.h"

// Sets default values
ASDPlayerCharacter::ASDPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 设置胶囊体碰撞通道
	GetCapsuleComponent()->SetCollisionProfileName(FName("PlayerProfile"));

	// 添加第一人称骨骼模型(获取组件蓝图)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshFirst(TEXT(
		"SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/SkMesh/FirstPlayer.FirstPlayer'"));
	// 实例化Mesh
	MeshFirst = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshFirst"));
	// 将Mesh赋值为组件蓝图
	MeshFirst->SetSkeletalMesh(StaticMeshFirst.Object);
	// 将MeshFirst添加到胶囊组件
	MeshFirst->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	// 设置可见
	MeshFirst->bOnlyOwnerSee = true;
	// 设置不渲染影子
	MeshFirst->bReceivesDecals = false;
	// 设置更新频率衰落
	MeshFirst->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	MeshFirst->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	// 设置MeshFirst碰撞属性
	MeshFirst->SetCollisionObjectType(ECC_Pawn);
	MeshFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshFirst->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 设置位移
	MeshFirst->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	MeshFirst->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, -90.f)));

	// 获取第一人称动作蓝图并添加
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimFirst(TEXT(
		"AnimBlueprint'/Game/Blueprint/Player/FirstPlayerAnimation.FirstPlayerAnimation_C'"));
	MeshFirst->AnimClass = StaticAnimFirst.Class;


	// 给Mesh添加默认骨骼模型
	// 加载Component
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshThird(TEXT(
		"SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Player/SkMesh/Player.Player'"));
	// 将其赋值给该类的Mesh
	GetMesh()->SetSkeletalMesh(StaticMeshThird.Object);

	// 设置Mesh属性
	GetMesh()->bOnlyOwnerSee = true;
	GetMesh()->bReceivesDecals = false;	// 允许贴花(别的物体反射到其表面)
	GetMesh()->SetCollisionObjectType(ECC_Pawn);	// 设置碰撞类型
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 根组件完成碰撞，该组件不碰撞
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);	// 碰撞忽略
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));	// 设置相对位置
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, -90.f)));	// 传入欧拉角，转为四元数，设置相对旋转

	// 给骨骼模型添加相应的动画蓝图
	// 获取第三人称的动作蓝图并添加
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimThird(TEXT(
		"AnimBlueprint'/Game/Blueprint/Player/ThirdPlayerAnimation.ThirdPlayerAnimation_C'"));
	GetMesh()->AnimClass = StaticAnimThird.Class;


	// 摄像机手臂
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	// 设置距离
	CameraBoom->TargetArmLength = 300.f;
	// 设置偏移
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 60.f);
	// 绑定Controller的旋转
	CameraBoom->bUsePawnControlRotation = true;

	// 初始化为第三人称摄像机
	ThirdCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdCamera"));
	// 组件绑定
	ThirdCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// 设置ThirdCamera不跟随控制器的旋转
	ThirdCamera->bUsePawnControlRotation = false;	// 摄像机组件绑定到了CameraBoom，而CameraBoom自会跟随旋转

	// 初始化第一人称摄像机
	FirstCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCamera"));
	// 组件绑定
	FirstCamera->SetupAttachment(RootComponent);
	// 设置跟随Controller的旋转
	FirstCamera->bUsePawnControlRotation = true;
	// 设置位置偏移
	FirstCamera->AddLocalOffset(FVector(0.f, 0.f, 60.f));


	// 设置默认第三人称摄像机
	FirstCamera->SetActive(false);
	ThirdCamera->SetActive(true);

	// 不显示第一人称模型
	GetMesh()->SetOwnerNoSee(false);	// 第三人称模型不可见为false
	MeshFirst->SetOwnerNoSee(true);		// 第一人称模型不可见为true

	// 实例化手上物品
	HandObject = CreateDefaultSubobject<UChildActorComponent>(TEXT("HandObject"));

	// 初始化参数
	BaseLookUpRate = 45.f;	// 镜头上下移动速度
	BaseTurnRate = 45.f;	// 镜头左右移动速度

	// 设置初始化速度为150.f
	GetCharacterMovement()->MaxWalkSpeed = 250.f;

	// 初始化为第三人称
	GameView = EGameViewMode::Third;

	// 上半身动作初始为无动作
	UpperType = EUpperBody::None;

	// 允许切换视角
	IsAllowSwitchView = true;

	// 允许输入
	IsInputLocked = false;

	// 初始为未攻击
	IsAttack = false;
}

// Called when the game starts or when spawned
void ASDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 添加控制器引用
	SPController = Cast<ASDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	// 把手持物品组件绑定到第三人称模型右手插槽上
	HandObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale
		, FName("RHSocket"));

	// 添加Actor到HandObject
	HandObject->SetChildActorClass(ASDHandObjcet::SpawnHandObject(0));
}

// Called every frame
void ASDPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 调用父类的同名函数
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 如果PlayerInputComponent组件存在则添加操作绑定
	check(PlayerInputComponent);
	
	// 轴映射绑定调用函数
	PlayerInputComponent->BindAxis("MoveForward", this, &ASDPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASDPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ASDPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASDPlayerCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASDPlayerCharacter::TurnAtRate);

	// 操作映射绑定调用函数
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASDPlayerCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASDPlayerCharacter::OnStopJump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASDPlayerCharacter::OnStartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASDPlayerCharacter::OnStopRun);

}

void ASDPlayerCharacter::ChangeView(EGameViewMode::Type NewGameView)
{
	GameView = NewGameView;
	switch (GameView)
	{
	case EGameViewMode::First:
		FirstCamera->SetActive(true);
		ThirdCamera->SetActive(false);
		MeshFirst->SetOwnerNoSee(false);
		GetMesh()->SetOwnerNoSee(true);
		// 修改手持物品绑定位置
		HandObject->AttachToComponent(MeshFirst, FAttachmentTransformRules::SnapToTargetNotIncludingScale
			, FName("RHSocket"));
		break;
	case EGameViewMode::Third:
		FirstCamera->SetActive(false);
		ThirdCamera->SetActive(true);
		MeshFirst->SetOwnerNoSee(true);
		GetMesh()->SetOwnerNoSee(false);
		HandObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale
			, FName("RHSocket"));
		break;
	}
}

void ASDPlayerCharacter::ChangeHandObject(TSubclassOf<class AActor> HandObjectClass)
{
	// 设置物品到HandObject
	HandObject->SetChildActorClass(HandObjectClass);
}

void ASDPlayerCharacter::ChangeHandObjectDetect(bool IsOpen)
{
	// 获取手上物品
	ASDHandObjcet* HandObjectClass = Cast<ASDHandObjcet>(HandObject->GetChildActor());
	if (HandObjectClass) HandObjectClass->ChangeOverlayDetect(IsOpen);

	// 如果手持物品则视为进入攻击状态
	IsAttack = IsOpen;
}

void ASDPlayerCharacter::RenderHandObject(bool IsRender)
{
	// 如果手上物品不存在则直接返回
	if (!HandObject->GetChildActor()) return;
	// 如果存在则让其消失
	HandObject->GetChildActor()->SetActorHiddenInGame(!IsRender);
}

void ASDPlayerCharacter::PlayerThrowObject(int ObjectID, int Num)
{
	// 将物品从玩家正向扔出
	if (GetWorld()) {
		for (int i = 0; i < Num; i++) {
			// 生成掉落物资源
			ASDFlobObject* FlobObject = GetWorld()->SpawnActor<ASDFlobObject>(GetActorLocation()
				+ FVector(0.f, 0.f, 50.f), FRotator::ZeroRotator);
			// 调用丢弃物初始化
			FlobObject->ThrowFlobObject(ObjectID, GetActorRotation().Yaw);
		}
	}
}

bool ASDPlayerCharacter::IsPackageFree(int ObjectID)
{
	return SDPackageManager::Get()->SearchFreeSpace(ObjectID);
}

void ASDPlayerCharacter::AddPackageObject(int ObjectID)
{
	SDPackageManager::Get()->AddObject(ObjectID);
}

void ASDPlayerCharacter::EatUpEvent()
{
	if (!SPController->SPState) {
		return;
	}
	// 如果成功吃掉东西
	if (SDPackageManager::Get()->EatUpEvent(SPController->SPState->CurrentShortcutIndex)) {
		SPController->SPState->PromoteHunger();
	}
}

FVector ASDPlayerCharacter::GetCameraLocation()
{
	switch (GameView)
	{
	case EGameViewMode::First:
		return FirstCamera->GetComponentLocation();
	case EGameViewMode::Third:
		return ThirdCamera->GetComponentLocation();
	}
	return this->GetActorLocation();
}

bool ASDPlayerCharacter::IsPlayerDead()
{
	if (SPController->SPState) return SPController->SPState->IsPlayerDead();
	return false;
}

void ASDPlayerCharacter::TakeDamage(int DamageVal)
{
	if (SPController->SPState) SPController->SPState->TakeDamage(DamageVal);
}

void ASDPlayerCharacter::MoveForward(float value)
{
	if (IsInputLocked) {
		return;
	}

	if (value != 0.f && Controller) {
		// 获得自身方向
		const FRotator Rotation = Controller->GetControlRotation();
		// 计算方向向量x轴上的方向移动方向
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// 传入移动方向和距离改变位置
		AddMovementInput(Direction, value);
	}
}

void ASDPlayerCharacter::MoveRight(float value)
{
	if (IsInputLocked) {
		return;
	}

	if (value != 0.f) {
		// 获得自身旋转(四元数)
		const FQuat Rotation = GetActorQuat();
		// 计算移动方向(自身旋转的y轴方向)
		FVector Direction = FQuatRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// 传入移动方向和距离改变位置
		AddMovementInput(Direction, value);
	}
}

void ASDPlayerCharacter::LookUpAtRate(float value)
{
	if (IsInputLocked) {
		return;
	}

	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASDPlayerCharacter::Turn(float value)
{
	if (IsInputLocked) {
		return;
	}

	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASDPlayerCharacter::TurnAtRate(float value)
{
	if (IsInputLocked) {
		return;
	}

	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASDPlayerCharacter::OnStartJump()
{
	if (IsInputLocked) {
		return;
	}

	bPressedJump = true;
}

void ASDPlayerCharacter::OnStopJump()
{
	if (IsInputLocked) {
		return;
	}

	bPressedJump = false;
	StopJumping();
}

void ASDPlayerCharacter::OnStartRun()
{
	if (IsInputLocked) {
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ASDPlayerCharacter::OnStopRun()
{
	if (IsInputLocked) {
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = 250.f;
}
