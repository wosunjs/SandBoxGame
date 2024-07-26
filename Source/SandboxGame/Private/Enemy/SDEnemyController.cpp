// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SDEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SDPlayerCharacter.h"
#include "Enemy/SDEnemyCharacter.h"

#include "AI/SDEnemyBlackboard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASDEnemyController::ASDEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASDEnemyController::OnPossess(APawn* InPawn)
{
	// ���ø���(����ΪOnPossess)
	Super::OnPossess(InPawn);

	// Possess����Beginplay֮ǰ���У�����ʵ������ɫ
	SECharacter = Cast<ASDEnemyCharacter>(GetPawn());

	// ��ȡ��Ϊ����Դ
	UBehaviorTree* StaticBehaviorTreeObject = LoadObject<UBehaviorTree>(NULL, TEXT(
		"BehaviorTree'/Game/Blueprint/Enemy/EnemyBehaviorTree.EnemyBehaviorTree'"));
	// ������Ϊ����Դ(����AI��ʹ���Լ����Ƶ���Ϊ����Դ)
	UBehaviorTree* BehaviorTreeObject = DuplicateObject<UBehaviorTree>(StaticBehaviorTreeObject, NULL);
	// �����Դ�����Ƿ�ɹ�
	if (!BehaviorTreeObject) return;
	// ���ƺڰ���Դ
	BehaviorTreeObject->BlackboardAsset = DuplicateObject<USDEnemyBlackboard>((USDEnemyBlackboard*)
		StaticBehaviorTreeObject->BlackboardAsset, NULL);

	// �ڰ��
	BlackboardComp = Blackboard;	// ��ʼ��Ϊ����ڰ�
	bool IsBindSuccess = false;
	// BlackboardΪ�ջ���Blackboard��BehaviorTreeObject->BlackboardAsset��ƥ��
	if (BehaviorTreeObject->BlackboardAsset && (Blackboard == nullptr || Blackboard->IsCompatibleWith(
		BehaviorTreeObject->BlackboardAsset) == false)) {
		IsBindSuccess = UseBlackboard(BehaviorTreeObject->BlackboardAsset, BlackboardComp);
	}

	// ����ڰ�󶨳ɹ�����ָ����Ϊ��������
	if (IsBindSuccess) {
		BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
		if (!BehaviorTreeComp) {
			BehaviorTreeComp = NewObject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
			BehaviorTreeComp->RegisterComponent();
		}
		BrainComponent = BehaviorTreeComp;
		check(BehaviorTreeComp != NULL);
		
		// ������Ϊ��
		BehaviorTreeComp->StartTree(*BehaviorTreeObject, EBTExecutionMode::Looped);

		// ����״̬ΪѲ��
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Patrol);

		// ����Ѳ���ٶ�
		SECharacter->SetMaxSpeed(100.f);
	}

	// ��д��������ueɾ������ģ�Ͳ����·��������У������񱨴�
}

void ASDEnemyController::OnUnPossess()
{
	Super::OnUnPossess();

	// ֹͣ��Ϊ��
	if (BehaviorTreeComp) BehaviorTreeComp->StopTree();
}

void ASDEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ÿ֡����Ѫ�����³���
	if (SECharacter && SPCharacter) SECharacter->UpdateHPBarRotation(SPCharacter->GetCameraLocation());

}

void ASDEnemyController::BeginPlay()
{
	// ���ø���
	Super::BeginPlay();

	// ��ʼ�����ָ��???���Ե���Ҽ�⵽ʱ�ٸ�ֵ
	SPCharacter = Cast<ASDPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GWorld, 0));

	// ��ʼ�������ɫָ��
	if (!SECharacter) SECharacter = Cast<ASDEnemyCharacter>(GetPawn());

	// ��ʼδ�������
	IsLockPlayer = false;

	// ����ί��
	FTimerDelegate EPDisDele = FTimerDelegate::CreateUObject(this, &ASDEnemyController::UpdateStateParam);
	GetWorld()->GetTimerManager().SetTimer(EPDisHandle, EPDisDele, 0.3f, true);		// ����ÿ0.3sִ��һ��

	// Ѫ���ٷֱȳ�ʼ��Ϊ100%
	HPRatio = 1;

	// ��������״̬
	IsAllowHurt = false;
	HurtTimerCount = 0.f;
}

FVector ASDEnemyController::GetPlayerLocation() const
{
	// ������ָ����ڣ��������λ��
	if (SPCharacter) return SPCharacter->GetActorLocation();
	return FVector::ZeroVector;
}

bool ASDEnemyController::IsPlayerDead()
{
	if (SPCharacter) return SPCharacter->IsPlayerDead();
	return false;
}

void ASDEnemyController::OnSeePlayer(APawn* PlayerChar)
{
	// ����Ѿ����������or���������������ִ��
	if (IsLockPlayer || IsPlayerDead()) return;

	// �����������
	IsLockPlayer = true;
	// �޸�Ԥ״̬Ϊ׷��
	BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Chase);
	// �޸�����ٶ�Ϊ300
	SECharacter->SetMaxSpeed(300.f);

	// AI��������ȡ���ָ��
	//SPCharacter = Cast<ASDPlayerCharacter>(PlayerChar);

}

void ASDEnemyController::LoosePlayer()
{
	IsLockPlayer = false;
}

void ASDEnemyController::UpdateStateParam()
{
	// ��������ҵľ�������
	if (EPDisList.Num() < 6) {
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}
	else {
		EPDisList.RemoveAt(0);
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}

	// �����������У�ÿ6s�������һ������״̬
	if (HurtTimerCount < 6.f) {
		HurtTimerCount += 0.3f;		// �ü�ʱ����0.3s����һ��
	}
	else {
		HurtTimerCount = 0.f;
		IsAllowHurt = true;
	}

}

bool ASDEnemyController::IsPlayerAway()
{
	if (!IsLockPlayer || !SPCharacter || EPDisList.Num() < 6 || IsPlayerDead()) return false;
	int BiggerNum = 0;
	float LastDis = -1.f;
	// ��������г���3��Զ�����(��������)�����ж�ΪԶ��
	for (TArray<float>::TIterator It(EPDisList); It; ++It) {
		if (*It > LastDis) BiggerNum += 1;
		LastDis = *It;
	}
	return BiggerNum > 3;
}

UObject* ASDEnemyController::GetPlayerPawn()
{
	return SPCharacter;
}

void ASDEnemyController::ResetProcess(bool IsFinish)
{
	// �޸����״̬
	BlackboardComp->SetValueAsBool("ProcessFinish", IsFinish);
}

void ASDEnemyController::UpdateDamageRatio(float HPRatioVal)
{
	// ����Ѫ���ٷֱ�
	HPRatio = HPRatioVal;
	// �޸�״̬Ϊ����
	if (IsAllowHurt) {
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)(EEnemyAIState::ES_Hurt));
		IsAllowHurt = false;
	}
}

void ASDEnemyController::FinishStateHurt()
{
	// ���û��������ң��ܻ���ֱ���������
	if (!IsLockPlayer) IsLockPlayer = true;

	// ���Ѫ����0.2f���£�ֱ������
	if (HPRatio < 0.2f) {
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Escape);
	}
	else {
		// ���������
		FRandomStream Stream;
		// �����µ��������
		Stream.GenerateNewSeed();
		// �����һ���������
		int ActionRatio = Stream.RandRange(0, 10);
		// 30%�ļ��ʽ������״̬
		if (ActionRatio < 4) {
			// �������״̬
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Defence);
		}
		else {
			// ���빥��״̬
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
		}
	}
}

void ASDEnemyController::FinishDefenceState()
{
	// ����״̬���
	ResetProcess(true);
	// ֹͣ��������
	SECharacter->StopDefence();

	// ͨ���������˼�ľ��������µ�״̬
	float SEToSP = FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation());
	// ������Ϊ����״̬�Ҿ���С��200���������,������������״̬
	if (SPCharacter->IsAttack && SEToSP < 200.f) {
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Defence);
	}
	else {
		// ���Ѫ������20%������һ����������
		if (HPRatio < 0.2f) {
			// ���������
			FRandomStream Stream;
			// �����µ��������
			Stream.GenerateNewSeed();
			// �����һ���������
			int ActionRatio = Stream.RandRange(0, 10);
			// 30%�ļ��ʽ�������״̬
			if (ActionRatio < 4) {
				// ���빥������
				BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Escape);
			}
			else {
				BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
			}
		}
		// ������빥��
		else {
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
		}
	}
}

void ASDEnemyController::EnemyDead()
{
	// ֹͣ��Ϊ��
	if (BehaviorTreeComp) BehaviorTreeComp->StopTree(EBTStopMode::Safe);

	// ע��ʱ�亯��
	if (EPDisHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(EPDisHandle);
}

