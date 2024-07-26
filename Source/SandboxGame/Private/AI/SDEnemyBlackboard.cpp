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

	// Ŀ�ĵ�
	FBlackboardEntry Destination;	// �����ڰ����
	Destination.EntryName = FName("Destination");	// ָ���ڰ����������
	UBlackboardKeyType_Vector* DestinationKeyType = NewObject<UBlackboardKeyType_Vector>();
	Destination.KeyType = DestinationKeyType;	// ָ���ڰ��������

	// ����״̬
	FBlackboardEntry EnemyState;
	EnemyState.EntryName = FName("EnemyState");
	UBlackboardKeyType_Enum* EnemyStateKeyType = NewObject<UBlackboardKeyType_Enum>();
	EnemyStateKeyType->EnumType = FindObject<UEnum>(ANY_PACKAGE, *FString("EEnemyAIState"), true);	// ͨ�����䴴����Ӧ���Ƶ�UEnumʵ��
	EnemyStateKeyType->EnumName = FString("EEnemyAIState");
	EnemyState.KeyType = EnemyStateKeyType;

	// �ȴ�ʱ��
	FBlackboardEntry WaitTime;
	WaitTime.EntryName = FName("WaitTime");
	WaitTime.KeyType = NewObject<UBlackboardKeyType_Float>();

	// ��������
	FBlackboardEntry AttackType;
	AttackType.EntryName = FName("AttackType");
	UBlackboardKeyType_Enum* AttackTypeKeyType = NewObject<UBlackboardKeyType_Enum>();
	AttackTypeKeyType->EnumType = FindObject<UEnum>(ANY_PACKAGE, *FString("EEnemyAttakType"), true);
	AttackTypeKeyType->EnumName = FString("EEnemyAttakType");
	AttackType.KeyType = AttackTypeKeyType;

	// ���ָ��
	FBlackboardEntry PlayerPawn;
	PlayerPawn.EntryName = FName("PlayerPawn");
	UBlackboardKeyType_Object* PlayerPawnKeyType = NewObject<UBlackboardKeyType_Object>();
	PlayerPawnKeyType->BaseClass = ASDPlayerCharacter::StaticClass();
	PlayerPawn.KeyType = PlayerPawnKeyType;

	// ĳһ�������Ƿ����
	FBlackboardEntry ProcessFinish;
	ProcessFinish.EntryName = FName("ProcessFinish");
	ProcessFinish.KeyType = NewObject<UBlackboardKeyType_Bool>();


	// ��ӵ��ڰ�����
	Keys.Add(Destination);
	Keys.Add(EnemyState);
	Keys.Add(WaitTime);
	Keys.Add(AttackType);
	Keys.Add(PlayerPawn);
	Keys.Add(ProcessFinish);
}
