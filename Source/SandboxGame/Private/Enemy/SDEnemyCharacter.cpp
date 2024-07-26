// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SDEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "Enemy/SDEnemyController.h"
#include "Enemy/SDEnemyAnim.h"
#include "EnemyTool/SDEnemyTool.h"
#include "UI/Widget/SSDEnemyHPWidget.h"
#include "Components/WidgetComponent.h"

#include "common/SDHelper.h"
#include "Data/SDDataHandle.h"
#include <Flob/SDFlobObject.h>

// Sets default values
ASDEnemyCharacter::ASDEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��AI������
	AIControllerClass = ASDEnemyController::StaticClass();

	// ������ײ��
	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyProfile"));
	// �����������
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// ���ģ��
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticEnemyMesh(TEXT(
		"SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Enemy/SkMesh/Enemy.Enemy'"));
	GetMesh()->SetSkeletalMesh(StaticEnemyMesh.Object);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, -90.f)));

	// ��Ӷ�����ͼ
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticEnemyAnim(TEXT(
		"AnimBlueprint'/Game/Blueprint/Enemy/Enemy_Animation.Enemy_Animation_C'"));
	//"AnimBlueprint'/Game/Blueprint/Enemy/Enemy_Animation.Enemy_Animation'"));
	GetMesh()->AnimClass = StaticEnemyAnim.Class;

	// ʵ�����ֳ���Ʒ���
	WeaponSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponSocket"));
	SheildSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("SheildSocket"));

	// ʵ����Ѫ��
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->AttachTo(RootComponent);

	// ʵ������֪���
	EnemySense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("EnemySense"));

	// ��������������Դ
	AnimDead_I = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), NULL, *FString(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Dead_I.Enemy_Dead_I'")));
	AnimDead_II = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), NULL, *FString(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Dead_II.Enemy_Dead_II'")));
}

// Called when the game starts or when spawned
void ASDEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ��ȡ��������
	SEAnim = Cast<USDEnemyAnim>(GetMesh()->GetAnimInstance());

	// ��ȡ����������
	SEController = Cast<ASDEnemyController>(GetController());
	
	// �������ֲ��(���ŵ��ǲ��漸�α仯)
	WeaponSocket->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale
		, FName("RHSocket"));
	SheildSocket->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale
		, FName("LHSocket"));

	// ����������Ʒ
	WeaponSocket->SetChildActorClass(ASDEnemyTool::SpawnEnemyWeapon());
	SheildSocket->SetChildActorClass(ASDEnemyTool::SpawnEnemySheild());

	// ��ʼ��Ѫ�����
	SAssignNew(HPBarWidget, SSDEnemyHPWidget);
	// ��ӵ�HPBar
	HPBar->SetSlateWidget(HPBarWidget);
	// ������ʽ
	HPBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HPBar->SetDrawSize(FVector2D(100.f, 10.f));

	// ��ʼ��Ѫ��
	HP = MaxHP;
	HPBarWidget->ChangeHP(HP / MaxHP);

	// ���˸�֪��������
	EnemySense->HearingThreshold = 0.f;
	EnemySense->LOSHearingThreshold = 0.f;
	EnemySense->SightRadius = 1000.f;
	EnemySense->SetPeripheralVisionAngle(55.f);
	EnemySense->bHearNoises = false;

	// �󶨿�����ҵķ���
	FScriptDelegate OnSeePlayerDele;
	OnSeePlayerDele.BindUFunction(this, "OnSeePlayer");	// ͨ��U�������䣬�Ժ�������ȡ����ָ��
	EnemySense->OnSeePawn.Add(OnSeePlayerDele);

	// ��ԴIDΪ3
	ResourceIndex = 3;
}

void ASDEnemyCharacter::CreateFlobObject()
{
	// ��ȡ����Դ����
	TSharedPtr<ResourceAttribute> ResourceAttr = *SDDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	// �������ɸö��������
	for (TArray<TArray<int>>::TIterator It(ResourceAttr->FlobObjectInfo); It; ++It) {
		// ������ɵ�����
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		// ��������
		int Num = Stream.RandRange((*It)[1], (*It)[2]);

		if (GetWorld()) {
			for (int i = 0; i < Num; i++) {
				// ���ɵ�����
				ASDFlobObject* FlobObject = GetWorld()->SpawnActor<ASDFlobObject>(GetActorLocation()
					+ FVector(0.f, 0.f, 20.f), FRotator::ZeroRotator);
				FlobObject->CreateFlobObject((*It)[0]);
			}
		}
	}
}

// Called every frame
void ASDEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASDEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASDEnemyCharacter::UpdateHPBarRotation(FVector SPCameraLocation)
{
	// ͨ������λ�ú����λ�ü���Ѫ����������
	FVector StartPos(GetActorLocation().X, GetActorLocation().Y, 0);
	FVector TargetPos(SPCameraLocation.X, SPCameraLocation.Y, 0.f);
	HPBar->SetWorldRotation(FRotationMatrix::MakeFromX(TargetPos - StartPos).Rotator());
}

void ASDEnemyCharacter::SetMaxSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float ASDEnemyCharacter::GetIdleWaitTime()
{
	// ����������������ڣ���ֱ�ӷ���3s
	if (!SEAnim) return 3.f;

	// ���������
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	// �����������
	int IdleType = Stream.RandRange(0, 2);
	// ��ȡ��������ʱ��
	float AnimLength = SEAnim->SetIdleType(IdleType);

	// ��������
	Stream.GenerateNewSeed();
	// ���ɴ����������Ŵ���
	int AnimCount = Stream.RandRange(1, 4);

	// ����ȫ���Ķ���ʱ��(����*����ʱ��)
	return AnimLength * AnimCount;
}

float ASDEnemyCharacter::PlayAttackAction(EEnemyAttakType AttackType)
{
	if (!SEAnim) return 0.f;
	// ���ع���ʱ��
	return SEAnim->PlayAttackAction(AttackType);
}

void ASDEnemyCharacter::UnderAttack(int DamageVal)
{
	// ���������˺�
	float ResDamage = DamageVal;
	// ��������������˺�����40%
	if (SEAnim && SEAnim->IsDefence) ResDamage *= 0.6;

	// Ѫ������
	HP = FMath::Clamp<float>(HP - ResDamage, 0.f, 500.f);
	// ����Ѫ��UI
	HPBarWidget->ChangeHP(HP / MaxHP);
	
	// ���Ѫ��Ϊ0
	if (HP == 0.f && !DeadHandle.IsValid()) {
		// ���߿���������,ֹͣ��Ϊ��
		SEController->EnemyDead();
		// ֹͣ���ж���
		SEAnim->StopAllAction();
		
		// ��ȡ��������ʱ��
		float DeadDuration = 0.f;
		// ���������������
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		int SelectIndex = Stream.RandRange(0, 1);
		if (SelectIndex == 0) {
			GetMesh()->PlayAnimation(AnimDead_I, false);
			DeadDuration = AnimDead_I->GetMaxCurrentTime() * 2;
		}
		else {
			GetMesh()->PlayAnimation(AnimDead_II, false);
			DeadDuration = AnimDead_II->GetMaxCurrentTime() * 2;
		}

		// ���ɵ�����
		CreateFlobObject();

		// ���ʱ��ί��
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASDEnemyCharacter::DestroyEvent);
		GetWorld()->GetTimerManager().SetTimer(DeadHandle, TimerDelegate, DeadDuration, false);
	}
	else {
		// ��֪�������ܵ��˺�
		if (SEController) SEController->UpdateDamageRatio(HP / MaxHP);
	}

}

float ASDEnemyCharacter::PlayHurtAction()
{
	if (!SEAnim) return 0.f;	// ��⶯����ͼ�Ƿ��ʼ��
	return SEAnim->PlayHurtAction();
}

void ASDEnemyCharacter::StartDefence()
{
	if (SEAnim) SEAnim->IsDefence = true;
}

void ASDEnemyCharacter::StopDefence()
{
	if (SEAnim) SEAnim->IsDefence = false;
}

void ASDEnemyCharacter::DestroyEvent()
{
	// ע��ʱ�亯��
	if (DeadHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(DeadHandle);
	// ��������
	GetWorld()->DestroyActor(this);
}

FText ASDEnemyCharacter::GetInfoText() const
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SDDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	switch (SDDataHandle::Get()->GetLocalizationCulture())
	{
	case ECultureTeam::EN:
		return ResourceAttr->EN;
		break;
	case ECultureTeam::ZH:
		return ResourceAttr->ZH;
		break;
	}
	return ResourceAttr->EN;
}

void ASDEnemyCharacter::ChangeWeaponDetect(bool IsOpen)
{
	// ����ֳ���Ʒ���ڣ��޸ļ��
	ASDEnemyTool* WeaponClass = Cast<ASDEnemyTool>(WeaponSocket->GetChildActor());

	if (WeaponClass) WeaponClass->ChangeOverlayDetect(IsOpen);
}

bool ASDEnemyCharacter::IsLockPlayer()
{
	if (SEController) return SEController->IsLockPlayer;
	return false;
}

void ASDEnemyCharacter::OnSeePlayer(APawn* PlayerChar)
{
	if (Cast<ASDPlayerCharacter>(PlayerChar)) {
		//SDHelper::Debug(FString("I see Player!"), 3.f);
		if (SEController) SEController->OnSeePlayer(PlayerChar);
	}
}
