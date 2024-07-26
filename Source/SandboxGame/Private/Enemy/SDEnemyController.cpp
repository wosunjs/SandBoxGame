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
	// 调用父类(这里为OnPossess)
	Super::OnPossess(InPawn);

	// Possess会在Beginplay之前运行，故先实例化角色
	SECharacter = Cast<ASDEnemyCharacter>(GetPawn());

	// 获取行为树资源
	UBehaviorTree* StaticBehaviorTreeObject = LoadObject<UBehaviorTree>(NULL, TEXT(
		"BehaviorTree'/Game/Blueprint/Enemy/EnemyBehaviorTree.EnemyBehaviorTree'"));
	// 复制行为树资源(单个AI会使用自己复制的行为树资源)
	UBehaviorTree* BehaviorTreeObject = DuplicateObject<UBehaviorTree>(StaticBehaviorTreeObject, NULL);
	// 检测资源复制是否成功
	if (!BehaviorTreeObject) return;
	// 复制黑板资源
	BehaviorTreeObject->BlackboardAsset = DuplicateObject<USDEnemyBlackboard>((USDEnemyBlackboard*)
		StaticBehaviorTreeObject->BlackboardAsset, NULL);

	// 黑板绑定
	BlackboardComp = Blackboard;	// 初始化为父类黑板
	bool IsBindSuccess = false;
	// Blackboard为空或者Blackboard与BehaviorTreeObject->BlackboardAsset不匹配
	if (BehaviorTreeObject->BlackboardAsset && (Blackboard == nullptr || Blackboard->IsCompatibleWith(
		BehaviorTreeObject->BlackboardAsset) == false)) {
		IsBindSuccess = UseBlackboard(BehaviorTreeObject->BlackboardAsset, BlackboardComp);
	}

	// 如果黑板绑定成功则尝试指定行为树并运行
	if (IsBindSuccess) {
		BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
		if (!BehaviorTreeComp) {
			BehaviorTreeComp = NewObject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
			BehaviorTreeComp->RegisterComponent();
		}
		BrainComponent = BehaviorTreeComp;
		check(BehaviorTreeComp != NULL);
		
		// 运行行为树
		BehaviorTreeComp->StartTree(*BehaviorTreeObject, EBTExecutionMode::Looped);

		// 设置状态为巡逻
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Patrol);

		// 设置巡逻速度
		SECharacter->SetMaxSpeed(100.f);
	}

	// 编写完后，需进入ue删除敌人模型并重新放入世界中，否则疯狂报错
}

void ASDEnemyController::OnUnPossess()
{
	Super::OnUnPossess();

	// 停止行为树
	if (BehaviorTreeComp) BehaviorTreeComp->StopTree();
}

void ASDEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 每帧调用血条更新朝向
	if (SECharacter && SPCharacter) SECharacter->UpdateHPBarRotation(SPCharacter->GetCameraLocation());

}

void ASDEnemyController::BeginPlay()
{
	// 调用父类
	Super::BeginPlay();

	// 初始化玩家指针???可以当玩家检测到时再赋值
	SPCharacter = Cast<ASDPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GWorld, 0));

	// 初始化自身角色指针
	if (!SECharacter) SECharacter = Cast<ASDEnemyCharacter>(GetPawn());

	// 初始未锁定玩家
	IsLockPlayer = false;

	// 创建委托
	FTimerDelegate EPDisDele = FTimerDelegate::CreateUObject(this, &ASDEnemyController::UpdateStateParam);
	GetWorld()->GetTimerManager().SetTimer(EPDisHandle, EPDisDele, 0.3f, true);		// 让其每0.3s执行一次

	// 血量百分比初始化为100%
	HPRatio = 1;

	// 设置受伤状态
	IsAllowHurt = false;
	HurtTimerCount = 0.f;
}

FVector ASDEnemyController::GetPlayerLocation() const
{
	// 如果玩家指针存在，返回玩家位置
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
	// 如果已经锁定了玩家or玩家已死亡，则不再执行
	if (IsLockPlayer || IsPlayerDead()) return;

	// 设置锁定玩家
	IsLockPlayer = true;
	// 修改预状态为追逐
	BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Chase);
	// 修改最大速度为300
	SECharacter->SetMaxSpeed(300.f);

	// AI控制器获取玩家指针
	//SPCharacter = Cast<ASDPlayerCharacter>(PlayerChar);

}

void ASDEnemyController::LoosePlayer()
{
	IsLockPlayer = false;
}

void ASDEnemyController::UpdateStateParam()
{
	// 更新与玩家的距离序列
	if (EPDisList.Num() < 6) {
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}
	else {
		EPDisList.RemoveAt(0);
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}

	// 更新受伤序列，每6s允许进入一次受伤状态
	if (HurtTimerCount < 6.f) {
		HurtTimerCount += 0.3f;		// 该计时任务0.3s运行一次
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
	// 如果序列中出现3次远离操作(距离拉开)，则判断为远离
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
	// 修改完成状态
	BlackboardComp->SetValueAsBool("ProcessFinish", IsFinish);
}

void ASDEnemyController::UpdateDamageRatio(float HPRatioVal)
{
	// 更新血量百分比
	HPRatio = HPRatioVal;
	// 修改状态为受伤
	if (IsAllowHurt) {
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)(EEnemyAIState::ES_Hurt));
		IsAllowHurt = false;
	}
}

void ASDEnemyController::FinishStateHurt()
{
	// 如果没有锁定玩家，受击后直接锁定玩家
	if (!IsLockPlayer) IsLockPlayer = true;

	// 如果血量在0.2f以下，直接逃跑
	if (HPRatio < 0.2f) {
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Escape);
	}
	else {
		// 创建随机流
		FRandomStream Stream;
		// 产生新的随机种子
		Stream.GenerateNewSeed();
		// 先随机一个动作类别
		int ActionRatio = Stream.RandRange(0, 10);
		// 30%的几率进入防御状态
		if (ActionRatio < 4) {
			// 进入防御状态
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Defence);
		}
		else {
			// 进入攻击状态
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
		}
	}
}

void ASDEnemyController::FinishDefenceState()
{
	// 设置状态完成
	ResetProcess(true);
	// 停止防御动作
	SECharacter->StopDefence();

	// 通过玩家与敌人间的距离设置新的状态
	float SEToSP = FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation());
	// 如果玩家为攻击状态且距离小于200则继续防御,否则跳出防御状态
	if (SPCharacter->IsAttack && SEToSP < 200.f) {
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Defence);
	}
	else {
		// 如果血量少于20%，则有一定几率逃跑
		if (HPRatio < 0.2f) {
			// 创建随机流
			FRandomStream Stream;
			// 产生新的随机种子
			Stream.GenerateNewSeed();
			// 先随机一个动作类别
			int ActionRatio = Stream.RandRange(0, 10);
			// 30%的几率进入逃跑状态
			if (ActionRatio < 4) {
				// 进入攻击逃跑
				BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Escape);
			}
			else {
				BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
			}
		}
		// 否则进入攻击
		else {
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
		}
	}
}

void ASDEnemyController::EnemyDead()
{
	// 停止行为树
	if (BehaviorTreeComp) BehaviorTreeComp->StopTree(EBTStopMode::Safe);

	// 注销时间函数
	if (EPDisHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(EPDisHandle);
}

