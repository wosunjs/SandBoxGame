// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SDEnemyBlackboard.h"
#include "Data/SDTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "Player/SDPlayerCharacter.h"

void USDEnemyBlackboard::PostLoad()
{
	Super::PostLoad();

	// 目的地
	FBlackboardEntry Destination;	// 创建黑板参数
	Destination.EntryName = FName("Destination");	// 指定黑板参数参数名
	UBlackboardKeyType_Vector* DestinationKeyType = NewObject<UBlackboardKeyType_Vector>();
	Destination.KeyType = DestinationKeyType;	// 指定黑板参数类型

	// 敌人状态
	FBlackboardEntry EnemyState;
	EnemyState.EntryName = FName("EnemyState");
	UBlackboardKeyType_Enum* EnemyStateKeyType = NewObject<UBlackboardKeyType_Enum>();
	EnemyStateKeyType->EnumType = FindObject<UEnum>(ANY_PACKAGE, *FString("EEnemyAIState"), true);	// 通过反射创建对应名称的UEnum实例
	EnemyStateKeyType->EnumName = FString("EEnemyAIState");
	EnemyState.KeyType = EnemyStateKeyType;

	// 等待时长
	FBlackboardEntry WaitTime;
	WaitTime.EntryName = FName("WaitTime");
	WaitTime.KeyType = NewObject<UBlackboardKeyType_Float>();

	// 攻击类型
	FBlackboardEntry AttackType;
	AttackType.EntryName = FName("AttackType");
	UBlackboardKeyType_Enum* AttackTypeKeyType = NewObject<UBlackboardKeyType_Enum>();
	AttackTypeKeyType->EnumType = FindObject<UEnum>(ANY_PACKAGE, *FString("EEnemyAttakType"), true);
	AttackTypeKeyType->EnumName = FString("EEnemyAttakType");
	AttackType.KeyType = AttackTypeKeyType;

	// 玩家指针
	FBlackboardEntry PlayerPawn;
	PlayerPawn.EntryName = FName("PlayerPawn");
	UBlackboardKeyType_Object* PlayerPawnKeyType = NewObject<UBlackboardKeyType_Object>();
	PlayerPawnKeyType->BaseClass = ASDPlayerCharacter::StaticClass();
	PlayerPawn.KeyType = PlayerPawnKeyType;

	// 某一个动作是否完成
	FBlackboardEntry ProcessFinish;
	ProcessFinish.EntryName = FName("ProcessFinish");
	ProcessFinish.KeyType = NewObject<UBlackboardKeyType_Bool>();


	// 添加到黑板数组
	Keys.Add(Destination);
	Keys.Add(EnemyState);
	Keys.Add(WaitTime);
	Keys.Add(AttackType);
	Keys.Add(PlayerPawn);
	Keys.Add(ProcessFinish);
}
