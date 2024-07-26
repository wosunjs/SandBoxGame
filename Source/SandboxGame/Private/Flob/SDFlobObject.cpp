// Fill out your copyright notice in the Description page of Project Settings.


#include "Flob/SDFlobObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Data/SDDataHandle.h"
#include "Data/SDTypes.h"
#include "Player/SDPlayerCharacter.h"
#include "TimerManager.h"


// Sets default values
ASDFlobObject::ASDFlobObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = Cast<USceneComponent>(BoxCollision);
	// 设置碰撞属性
	BoxCollision->SetCollisionProfileName(FName("FlobProfile"));
	// 启动物理模拟，产生碰撞时位移
	BoxCollision->SetSimulatePhysics(true);
	// 锁定旋转
	BoxCollision->SetConstraintMode(EDOFMode::Default);
	BoxCollision->GetBodyInstance()->bLockXRotation = true;
	BoxCollision->GetBodyInstance()->bLockYRotation = true;
	BoxCollision->GetBodyInstance()->bLockZRotation = true;
	// 设置碰撞体大小
	BoxCollision->SetBoxExtent(FVector(15.f));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);
	BaseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 设置变换
	BaseMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	BaseMesh->SetRelativeScale3D(FVector(0.3f));

	UMaterialInterface* StaticObjectIconMat = LoadObject<UMaterialInterface>(NULL, TEXT(
		"MaterialInstanceConstant'/Game/Material/FlobIconMatInst.FlobIconMatInst'"));
	// 动态创建材质
	ObjectIconMatDynamic = UMaterialInstanceDynamic::Create(StaticObjectIconMat, nullptr);

}

// Called when the game starts or when spawned
void ASDFlobObject::BeginPlay()
{
	Super::BeginPlay();
	
	// 检测世界是否存在
	if (!GetWorld()) {
		return;
	}

	// 注册检测事件：每秒检测一次是否有玩家在附近尝试拾取该掉落物
	FTimerDelegate DetectPlayerDele;
	DetectPlayerDele.BindUObject(this, &ASDFlobObject::DetectPlayer);
	// 每秒运行一次，循环运行，延迟3s运行
	GetWorld()->GetTimerManager().SetTimer(DetectTimer, DetectPlayerDele, 1.f, true, 3.f);

	// 注册销毁事件
	FTimerDelegate DestoryDele;
	DestoryDele.BindUObject(this, &ASDFlobObject::DestroyEvent);
	// 10s后自动触发销毁函数
	GetWorld()->GetTimerManager().SetTimer(DestoryTimer, DestoryDele, 60.f, false);

	// 初始拾取玩家为null
	SPCharacter = nullptr;
}

// Called every frame
void ASDFlobObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 一直旋转
	BaseMesh->AddLocalRotation(FRotator(DeltaTime * 60.f, 0.f, 0.f));

	// 如果检测到玩家
	if (SPCharacter) {
		// 靠近玩家
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), SPCharacter->GetActorLocation() 
			+ FVector(0.f, 0.f, 40.f), DeltaTime, 5.f));
		// 如果距离接近40以内
		if (FVector::Distance(GetActorLocation(), SPCharacter->GetActorLocation() 
			+ FVector(0.f, 0.f, 40.f)) < 40.f) {
			// 检查玩家背包是否有空间
			if (SPCharacter->IsPackageFree(ObjectIndex)) {
				// 添加对应物品到背包
				SPCharacter->AddPackageObject(ObjectIndex);
				// 销毁自己
				DestroyEvent();
			}
			else {
				// 如果玩家背包不为空，重置参数
				SPCharacter = nullptr;
				// 唤醒检测和销毁
				GetWorld()->GetTimerManager().UnPauseTimer(DetectTimer);
				GetWorld()->GetTimerManager().UnPauseTimer(DestoryTimer);
				// 开启碰撞
				BoxCollision->SetSimulatePhysics(true);
			}
		}
	}

}

void ASDFlobObject::CreateFlobObject(int ObjectID)
{
	// 指定ID
	ObjectIndex = ObjectID;

	// 渲染贴图
	RenderTexture();

	// 随机方向力
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int DirYaw = Stream.FRandRange(-180, 180);
	FRotator ForceRot = FRotator(0.f, DirYaw, 0.f);

	// 添加力
	BoxCollision->AddForce((FVector(0.f, 0.f, 4.f) + ForceRot.Vector()) * 100000.f);
}

void ASDFlobObject::ThrowFlobObject(int ObjectID, float DirYaw)
{
	// 指定ID
	ObjectIndex = ObjectID;

	// 渲染贴图
	RenderTexture();

	// 随机方向力
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	DirYaw += Stream.FRandRange(-30, 30);
	FRotator ForceRot = FRotator(0.f, DirYaw, 0.f);

	// 添加力
	BoxCollision->AddForce((FVector(0.f, 0.f, 2.f) + ForceRot.Vector()) * 120000.f);
}

void ASDFlobObject::RenderTexture()
{
	TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
	ObjectIconTex = LoadObject<UTexture>(NULL, *ObjectAttr->TexPath);
	ObjectIconMatDynamic->SetTextureParameterValue(FName("ObjectTex"), ObjectIconTex);
	BaseMesh->SetMaterial(0, ObjectIconMatDynamic);
}

void ASDFlobObject::DetectPlayer()
{
	// 检测世界是否存在
	if (!GetWorld()) {
		return;
	}

	// 保存检测结果
	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	//Params.bTraceComplex = true;

	// 进行动态检测，范围是200，检测成功的话返回true
	if (GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, ObjectParams
		, FCollisionShape::MakeSphere(200.f), Params)) {
		for (TArray<FOverlapResult>::TIterator It(Overlaps); It; ++It) {
			// 如果检测到了玩家
			if (Cast<ASDPlayerCharacter>(It->GetActor())) {
				SPCharacter = Cast<ASDPlayerCharacter>(It->GetActor());
				// 检测该玩家背包是否有空间
				if (SPCharacter->IsPackageFree(ObjectIndex)) {
					// 停止检测
					GetWorld()->GetTimerManager().PauseTimer(DetectTimer);
					// 停止销毁定时器
					GetWorld()->GetTimerManager().PauseTimer(DestoryTimer);
					// 关闭物理模拟
					BoxCollision->SetSimulatePhysics(false);
				}
				return;
			}
		}
	}
}

void ASDFlobObject::DestroyEvent()
{
	if (!GetWorld()) {
		return;
	}
	// 注销定时器
	GetWorld()->GetTimerManager().ClearTimer(DetectTimer);
	GetWorld()->GetTimerManager().ClearTimer(DestoryTimer);
	// 销毁自身
	GetWorld()->DestroyActor(this);

}
