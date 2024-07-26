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

	// ���ý�������ײͨ��
	GetCapsuleComponent()->SetCollisionProfileName(FName("PlayerProfile"));

	// ��ӵ�һ�˳ƹ���ģ��(��ȡ�����ͼ)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshFirst(TEXT(
		"SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/SkMesh/FirstPlayer.FirstPlayer'"));
	// ʵ����Mesh
	MeshFirst = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshFirst"));
	// ��Mesh��ֵΪ�����ͼ
	MeshFirst->SetSkeletalMesh(StaticMeshFirst.Object);
	// ��MeshFirst��ӵ��������
	MeshFirst->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	// ���ÿɼ�
	MeshFirst->bOnlyOwnerSee = true;
	// ���ò���ȾӰ��
	MeshFirst->bReceivesDecals = false;
	// ���ø���Ƶ��˥��
	MeshFirst->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	MeshFirst->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	// ����MeshFirst��ײ����
	MeshFirst->SetCollisionObjectType(ECC_Pawn);
	MeshFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshFirst->SetCollisionResponseToAllChannels(ECR_Ignore);
	// ����λ��
	MeshFirst->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	MeshFirst->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, -90.f)));

	// ��ȡ��һ�˳ƶ�����ͼ�����
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimFirst(TEXT(
		"AnimBlueprint'/Game/Blueprint/Player/FirstPlayerAnimation.FirstPlayerAnimation_C'"));
	MeshFirst->AnimClass = StaticAnimFirst.Class;


	// ��Mesh���Ĭ�Ϲ���ģ��
	// ����Component
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshThird(TEXT(
		"SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Player/SkMesh/Player.Player'"));
	// ���丳ֵ�������Mesh
	GetMesh()->SetSkeletalMesh(StaticMeshThird.Object);

	// ����Mesh����
	GetMesh()->bOnlyOwnerSee = true;
	GetMesh()->bReceivesDecals = false;	// ��������(������巴�䵽�����)
	GetMesh()->SetCollisionObjectType(ECC_Pawn);	// ������ײ����
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// ����������ײ�����������ײ
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);	// ��ײ����
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));	// �������λ��
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, -90.f)));	// ����ŷ���ǣ�תΪ��Ԫ�������������ת

	// ������ģ�������Ӧ�Ķ�����ͼ
	// ��ȡ�����˳ƵĶ�����ͼ�����
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimThird(TEXT(
		"AnimBlueprint'/Game/Blueprint/Player/ThirdPlayerAnimation.ThirdPlayerAnimation_C'"));
	GetMesh()->AnimClass = StaticAnimThird.Class;


	// ������ֱ�
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	// ���þ���
	CameraBoom->TargetArmLength = 300.f;
	// ����ƫ��
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 60.f);
	// ��Controller����ת
	CameraBoom->bUsePawnControlRotation = true;

	// ��ʼ��Ϊ�����˳������
	ThirdCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdCamera"));
	// �����
	ThirdCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// ����ThirdCamera���������������ת
	ThirdCamera->bUsePawnControlRotation = false;	// ���������󶨵���CameraBoom����CameraBoom�Ի������ת

	// ��ʼ����һ�˳������
	FirstCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCamera"));
	// �����
	FirstCamera->SetupAttachment(RootComponent);
	// ���ø���Controller����ת
	FirstCamera->bUsePawnControlRotation = true;
	// ����λ��ƫ��
	FirstCamera->AddLocalOffset(FVector(0.f, 0.f, 60.f));


	// ����Ĭ�ϵ����˳������
	FirstCamera->SetActive(false);
	ThirdCamera->SetActive(true);

	// ����ʾ��һ�˳�ģ��
	GetMesh()->SetOwnerNoSee(false);	// �����˳�ģ�Ͳ��ɼ�Ϊfalse
	MeshFirst->SetOwnerNoSee(true);		// ��һ�˳�ģ�Ͳ��ɼ�Ϊtrue

	// ʵ����������Ʒ
	HandObject = CreateDefaultSubobject<UChildActorComponent>(TEXT("HandObject"));

	// ��ʼ������
	BaseLookUpRate = 45.f;	// ��ͷ�����ƶ��ٶ�
	BaseTurnRate = 45.f;	// ��ͷ�����ƶ��ٶ�

	// ���ó�ʼ���ٶ�Ϊ150.f
	GetCharacterMovement()->MaxWalkSpeed = 250.f;

	// ��ʼ��Ϊ�����˳�
	GameView = EGameViewMode::Third;

	// �ϰ�������ʼΪ�޶���
	UpperType = EUpperBody::None;

	// �����л��ӽ�
	IsAllowSwitchView = true;

	// ��������
	IsInputLocked = false;

	// ��ʼΪδ����
	IsAttack = false;
}

// Called when the game starts or when spawned
void ASDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ��ӿ���������
	SPController = Cast<ASDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	// ���ֳ���Ʒ����󶨵������˳�ģ�����ֲ����
	HandObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale
		, FName("RHSocket"));

	// ���Actor��HandObject
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
	// ���ø����ͬ������
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���PlayerInputComponent�����������Ӳ�����
	check(PlayerInputComponent);
	
	// ��ӳ��󶨵��ú���
	PlayerInputComponent->BindAxis("MoveForward", this, &ASDPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASDPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ASDPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASDPlayerCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASDPlayerCharacter::TurnAtRate);

	// ����ӳ��󶨵��ú���
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
		// �޸��ֳ���Ʒ��λ��
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
	// ������Ʒ��HandObject
	HandObject->SetChildActorClass(HandObjectClass);
}

void ASDPlayerCharacter::ChangeHandObjectDetect(bool IsOpen)
{
	// ��ȡ������Ʒ
	ASDHandObjcet* HandObjectClass = Cast<ASDHandObjcet>(HandObject->GetChildActor());
	if (HandObjectClass) HandObjectClass->ChangeOverlayDetect(IsOpen);

	// ����ֳ���Ʒ����Ϊ���빥��״̬
	IsAttack = IsOpen;
}

void ASDPlayerCharacter::RenderHandObject(bool IsRender)
{
	// ���������Ʒ��������ֱ�ӷ���
	if (!HandObject->GetChildActor()) return;
	// ���������������ʧ
	HandObject->GetChildActor()->SetActorHiddenInGame(!IsRender);
}

void ASDPlayerCharacter::PlayerThrowObject(int ObjectID, int Num)
{
	// ����Ʒ����������ӳ�
	if (GetWorld()) {
		for (int i = 0; i < Num; i++) {
			// ���ɵ�������Դ
			ASDFlobObject* FlobObject = GetWorld()->SpawnActor<ASDFlobObject>(GetActorLocation()
				+ FVector(0.f, 0.f, 50.f), FRotator::ZeroRotator);
			// ���ö������ʼ��
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
	// ����ɹ��Ե�����
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
		// ���������
		const FRotator Rotation = Controller->GetControlRotation();
		// ���㷽������x���ϵķ����ƶ�����
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// �����ƶ�����;���ı�λ��
		AddMovementInput(Direction, value);
	}
}

void ASDPlayerCharacter::MoveRight(float value)
{
	if (IsInputLocked) {
		return;
	}

	if (value != 0.f) {
		// ���������ת(��Ԫ��)
		const FQuat Rotation = GetActorQuat();
		// �����ƶ�����(������ת��y�᷽��)
		FVector Direction = FQuatRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// �����ƶ�����;���ı�λ��
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
