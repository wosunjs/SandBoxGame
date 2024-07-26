// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/SDTypes.h"

#include "SDPlayerCharacter.generated.h"

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UChildActorComponent;
class ASDPlayerController;

UCLASS()
class SANDBOXGAME_API ASDPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASDPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 修改视角
	void ChangeView(EGameViewMode::Type NewGameView);

	// 修改当前手持物品
	void ChangeHandObject(TSubclassOf<class AActor> HandObjectClass);

	// 修改手持物品的碰撞检测是否开启
	void ChangeHandObjectDetect(bool IsOpen);

	// 是否渲染手持物品
	void RenderHandObject(bool IsRender);

	// 丢弃物品
	void PlayerThrowObject(int ObjectID, int Num);

	// 检查背包是否有空间
	bool IsPackageFree(int ObjectID);

	// 添加物品到背包
	void AddPackageObject(int ObjectID);

	// 吃东西调用事件,由Anim调用
	void EatUpEvent();

	// 获取角色目前相机
	FVector GetCameraLocation();

	// 获取是否已经死亡
	bool IsPlayerDead();

	// 受到伤害
	void TakeDamage(int DamageVal);

public:
	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	UCameraComponent* FirstCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	UCameraComponent* ThirdCamera;

	UPROPERTY(BlueprintReadWrite, Category = "SD")
	float RunSpeed = 375.f;		// 奔跑速度

	// 玩家控制器指针
	ASDPlayerController* SPController;

	// 当前的视角状态
	EGameViewMode::Type GameView;

	// 上半身动画状态
	EUpperBody::Type UpperType;

	// 是否允许切换视角
	bool IsAllowSwitchView;
	
	// 是否锁住输入
	bool IsInputLocked;

	// 是否在短时间内进行过攻击
	bool IsAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUpAtRate(float value);
	void Turn(float value);
	void TurnAtRate(float value);
	void OnStartJump();
	void OnStopJump();
	void OnStartRun();
	void OnStopRun();

private:
	// 第一人称的骨骼模型
	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	USkeletalMeshComponent* MeshFirst;

	// 手上物品
	UPROPERTY(VisibleDefaultsOnly, Category = "SD")
	UChildActorComponent* HandObject;

	// 旋转速度(比例)
	float BaseLookUpRate;
	float BaseTurnRate;


};
