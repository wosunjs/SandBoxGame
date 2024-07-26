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

	// 绑定AI控制器
	AIControllerClass = ASDEnemyController::StaticClass();

	// 设置碰撞体
	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyProfile"));
	// 开启交互检测
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// 添加模型
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticEnemyMesh(TEXT(
		"SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Enemy/SkMesh/Enemy.Enemy'"));
	GetMesh()->SetSkeletalMesh(StaticEnemyMesh.Object);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, -90.f)));

	// 添加动画蓝图
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticEnemyAnim(TEXT(
		"AnimBlueprint'/Game/Blueprint/Enemy/Enemy_Animation.Enemy_Animation_C'"));
	//"AnimBlueprint'/Game/Blueprint/Enemy/Enemy_Animation.Enemy_Animation'"));
	GetMesh()->AnimClass = StaticEnemyAnim.Class;

	// 实例化手持物品插槽
	WeaponSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponSocket"));
	SheildSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("SheildSocket"));

	// 实例化血条
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->AttachTo(RootComponent);

	// 实例化感知组件
	EnemySense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("EnemySense"));

	// 加载死亡动画资源
	AnimDead_I = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), NULL, *FString(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Dead_I.Enemy_Dead_I'")));
	AnimDead_II = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), NULL, *FString(
		"AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Dead_II.Enemy_Dead_II'")));
}

// Called when the game starts or when spawned
void ASDEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 获取动作引用
	SEAnim = Cast<USDEnemyAnim>(GetMesh()->GetAnimInstance());

	// 获取控制器引用
	SEController = Cast<ASDEnemyController>(GetController());
	
	// 绑定左右手插槽(附着但是不随几何变化)
	WeaponSocket->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale
		, FName("RHSocket"));
	SheildSocket->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale
		, FName("LHSocket"));

	// 给插槽添加物品
	WeaponSocket->SetChildActorClass(ASDEnemyTool::SpawnEnemyWeapon());
	SheildSocket->SetChildActorClass(ASDEnemyTool::SpawnEnemySheild());

	// 初始化血条组件
	SAssignNew(HPBarWidget, SSDEnemyHPWidget);
	// 添加到HPBar
	HPBar->SetSlateWidget(HPBarWidget);
	// 设置样式
	HPBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HPBar->SetDrawSize(FVector2D(100.f, 10.f));

	// 初始化血条
	HP = MaxHP;
	HPBarWidget->ChangeHP(HP / MaxHP);

	// 敌人感知参数设置
	EnemySense->HearingThreshold = 0.f;
	EnemySense->LOSHearingThreshold = 0.f;
	EnemySense->SightRadius = 1000.f;
	EnemySense->SetPeripheralVisionAngle(55.f);
	EnemySense->bHearNoises = false;

	// 绑定看到玩家的方法
	FScriptDelegate OnSeePlayerDele;
	OnSeePlayerDele.BindUFunction(this, "OnSeePlayer");	// 通过U函数反射，以函数名获取函数指针
	EnemySense->OnSeePawn.Add(OnSeePlayerDele);

	// 资源ID为3
	ResourceIndex = 3;
}

void ASDEnemyCharacter::CreateFlobObject()
{
	// 获取该资源对象
	TSharedPtr<ResourceAttribute> ResourceAttr = *SDDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	// 遍历生成该对象掉落物
	for (TArray<TArray<int>>::TIterator It(ResourceAttr->FlobObjectInfo); It; ++It) {
		// 随机生成的数量
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		// 生成数量
		int Num = Stream.RandRange((*It)[1], (*It)[2]);

		if (GetWorld()) {
			for (int i = 0; i < Num; i++) {
				// 生成掉落物
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
	// 通过敌人位置和玩家位置计算血条朝向向量
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
	// 如果待机动作不存在，则直接返回3s
	if (!SEAnim) return 3.f;

	// 创建随机流
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	// 随机待机动作
	int IdleType = Stream.RandRange(0, 2);
	// 获取待机动作时长
	float AnimLength = SEAnim->SetIdleType(IdleType);

	// 更新种子
	Stream.GenerateNewSeed();
	// 生成待机动作播放次数
	int AnimCount = Stream.RandRange(1, 4);

	// 返回全部的动画时长(次数*单次时长)
	return AnimLength * AnimCount;
}

float ASDEnemyCharacter::PlayAttackAction(EEnemyAttakType AttackType)
{
	if (!SEAnim) return 0.f;
	// 返回攻击时长
	return SEAnim->PlayAttackAction(AttackType);
}

void ASDEnemyCharacter::UnderAttack(int DamageVal)
{
	// 计算最终伤害
	float ResDamage = DamageVal;
	// 如果开启防御则伤害减少40%
	if (SEAnim && SEAnim->IsDefence) ResDamage *= 0.6;

	// 血量更新
	HP = FMath::Clamp<float>(HP - ResDamage, 0.f, 500.f);
	// 更新血条UI
	HPBarWidget->ChangeHP(HP / MaxHP);
	
	// 如果血量为0
	if (HP == 0.f && !DeadHandle.IsValid()) {
		// 告诉控制器死亡,停止行为树
		SEController->EnemyDead();
		// 停止所有动画
		SEAnim->StopAllAction();
		
		// 获取死亡动作时间
		float DeadDuration = 0.f;
		// 产生随机死亡动画
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

		// 生成掉落物
		CreateFlobObject();

		// 添加时间委托
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASDEnemyCharacter::DestroyEvent);
		GetWorld()->GetTimerManager().SetTimer(DeadHandle, TimerDelegate, DeadDuration, false);
	}
	else {
		// 告知控制器受到伤害
		if (SEController) SEController->UpdateDamageRatio(HP / MaxHP);
	}

}

float ASDEnemyCharacter::PlayHurtAction()
{
	if (!SEAnim) return 0.f;	// 检测动画蓝图是否初始化
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
	// 注销时间函数
	if (DeadHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(DeadHandle);
	// 销毁自身
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
	// 如果手持物品存在，修改检测
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
