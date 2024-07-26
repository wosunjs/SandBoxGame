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
	// ������ײ����
	BoxCollision->SetCollisionProfileName(FName("FlobProfile"));
	// ��������ģ�⣬������ײʱλ��
	BoxCollision->SetSimulatePhysics(true);
	// ������ת
	BoxCollision->SetConstraintMode(EDOFMode::Default);
	BoxCollision->GetBodyInstance()->bLockXRotation = true;
	BoxCollision->GetBodyInstance()->bLockYRotation = true;
	BoxCollision->GetBodyInstance()->bLockZRotation = true;
	// ������ײ���С
	BoxCollision->SetBoxExtent(FVector(15.f));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);
	BaseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// ���ñ任
	BaseMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	BaseMesh->SetRelativeScale3D(FVector(0.3f));

	UMaterialInterface* StaticObjectIconMat = LoadObject<UMaterialInterface>(NULL, TEXT(
		"MaterialInstanceConstant'/Game/Material/FlobIconMatInst.FlobIconMatInst'"));
	// ��̬��������
	ObjectIconMatDynamic = UMaterialInstanceDynamic::Create(StaticObjectIconMat, nullptr);

}

// Called when the game starts or when spawned
void ASDFlobObject::BeginPlay()
{
	Super::BeginPlay();
	
	// ��������Ƿ����
	if (!GetWorld()) {
		return;
	}

	// ע�����¼���ÿ����һ���Ƿ�������ڸ�������ʰȡ�õ�����
	FTimerDelegate DetectPlayerDele;
	DetectPlayerDele.BindUObject(this, &ASDFlobObject::DetectPlayer);
	// ÿ������һ�Σ�ѭ�����У��ӳ�3s����
	GetWorld()->GetTimerManager().SetTimer(DetectTimer, DetectPlayerDele, 1.f, true, 3.f);

	// ע�������¼�
	FTimerDelegate DestoryDele;
	DestoryDele.BindUObject(this, &ASDFlobObject::DestroyEvent);
	// 10s���Զ��������ٺ���
	GetWorld()->GetTimerManager().SetTimer(DestoryTimer, DestoryDele, 60.f, false);

	// ��ʼʰȡ���Ϊnull
	SPCharacter = nullptr;
}

// Called every frame
void ASDFlobObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// һֱ��ת
	BaseMesh->AddLocalRotation(FRotator(DeltaTime * 60.f, 0.f, 0.f));

	// �����⵽���
	if (SPCharacter) {
		// �������
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), SPCharacter->GetActorLocation() 
			+ FVector(0.f, 0.f, 40.f), DeltaTime, 5.f));
		// �������ӽ�40����
		if (FVector::Distance(GetActorLocation(), SPCharacter->GetActorLocation() 
			+ FVector(0.f, 0.f, 40.f)) < 40.f) {
			// �����ұ����Ƿ��пռ�
			if (SPCharacter->IsPackageFree(ObjectIndex)) {
				// ��Ӷ�Ӧ��Ʒ������
				SPCharacter->AddPackageObject(ObjectIndex);
				// �����Լ�
				DestroyEvent();
			}
			else {
				// �����ұ�����Ϊ�գ����ò���
				SPCharacter = nullptr;
				// ���Ѽ�������
				GetWorld()->GetTimerManager().UnPauseTimer(DetectTimer);
				GetWorld()->GetTimerManager().UnPauseTimer(DestoryTimer);
				// ������ײ
				BoxCollision->SetSimulatePhysics(true);
			}
		}
	}

}

void ASDFlobObject::CreateFlobObject(int ObjectID)
{
	// ָ��ID
	ObjectIndex = ObjectID;

	// ��Ⱦ��ͼ
	RenderTexture();

	// ���������
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int DirYaw = Stream.FRandRange(-180, 180);
	FRotator ForceRot = FRotator(0.f, DirYaw, 0.f);

	// �����
	BoxCollision->AddForce((FVector(0.f, 0.f, 4.f) + ForceRot.Vector()) * 100000.f);
}

void ASDFlobObject::ThrowFlobObject(int ObjectID, float DirYaw)
{
	// ָ��ID
	ObjectIndex = ObjectID;

	// ��Ⱦ��ͼ
	RenderTexture();

	// ���������
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	DirYaw += Stream.FRandRange(-30, 30);
	FRotator ForceRot = FRotator(0.f, DirYaw, 0.f);

	// �����
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
	// ��������Ƿ����
	if (!GetWorld()) {
		return;
	}

	// ��������
	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	//Params.bTraceComplex = true;

	// ���ж�̬��⣬��Χ��200�����ɹ��Ļ�����true
	if (GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, ObjectParams
		, FCollisionShape::MakeSphere(200.f), Params)) {
		for (TArray<FOverlapResult>::TIterator It(Overlaps); It; ++It) {
			// �����⵽�����
			if (Cast<ASDPlayerCharacter>(It->GetActor())) {
				SPCharacter = Cast<ASDPlayerCharacter>(It->GetActor());
				// ������ұ����Ƿ��пռ�
				if (SPCharacter->IsPackageFree(ObjectIndex)) {
					// ֹͣ���
					GetWorld()->GetTimerManager().PauseTimer(DetectTimer);
					// ֹͣ���ٶ�ʱ��
					GetWorld()->GetTimerManager().PauseTimer(DestoryTimer);
					// �ر�����ģ��
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
	// ע����ʱ��
	GetWorld()->GetTimerManager().ClearTimer(DetectTimer);
	GetWorld()->GetTimerManager().ClearTimer(DestoryTimer);
	// ��������
	GetWorld()->DestroyActor(this);

}
