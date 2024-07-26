// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/SDTypes.h"
#include "SDPlayerController.generated.h"

class ASDPlayerCharacter;
class ASDPlayerState;

// 修改准星委托
DECLARE_DELEGATE_TwoParams(FUpdatePointer, bool, float)

// 显示UI委托
DECLARE_DELEGATE_TwoParams(FShowGameUI, EGameUIType::Type, EGameUIType::Type)

// 修改小地图视野范围委托
DECLARE_DELEGATE_OneParam(FUpdateMiniMapWidth, int)

/**
 * 
 */
UCLASS()
class SANDBOXGAME_API ASDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASDPlayerController();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	// 对Character的手持物品进行更改，这个函数在Playerstate内会调用
	void ChangeHandObject();

public:
	// 获取玩家角色
	ASDPlayerCharacter* SPCharacter;

	// 获取玩家控制器
	ASDPlayerState* SPState;

	// 实时修改准星的委托,注册的函数是PointerWidget的UpdatePointer
	FUpdatePointer UpdatePointer;

	// 显示游戏UI界面委托,绑定的方法是GameHUDWidget的ShowGameUI 
	FShowGameUI ShowGameUI;

	// 为小地图更新委托声明变量,注册的函数是ASDSceneCapture2D的UpdateMiniMapWidth
	FUpdateMiniMapWidth UpdateMiniMapWidth;

protected:
	virtual void BeginPlay() override;

private:
	// 切换视角
	void ChangeView();

	// 鼠标按键事件
	void LeftEventStart();
	void LeftEventStop();
	void RightEventStart();
	void RightEventStop();

	// 鼠标滑轮滚事件
	void ScrollUpEvent();
	void ScrollDownEvent();

	// 修改预动作
	void ChangePreUpperType(EUpperBody::Type RightType);
	
	// 射线检测结果
	FHitResult RayGetHitResult(FVector TraceStart, FVector TraceEnd);

	// 射线绘制
	void DrawRayLine(FVector StartPos, FVector EndPos, float Duration);

	// 进行射线检测
	void RunRayCast();

	// 行为状态机
	void StateMachine();

	// Esc按下事件
	void EscEvent();

	// E键背包事件
	void PackageEvent();

	// T键聊天室事件
	void ChatRoomEvent();

	// 转换输入模式，true为游戏模式，false为混合模式
	void SwitchInputMode(bool IsGmaeOnly);

	// 设置锁住输入
	void LockedInput(bool IsLocked);

	// 小地图缩放事件
	void AddMapSizeStart();
	void AddMapSizeStop();
	void ReduceMapSizeStart();
	void ReduceMapSizeStop();

	// 定义小地图缩放的Tick函数，让其每帧调用执行
	void TickMiniMap();

private:
	// 左键预动作
	EUpperBody::Type LeftUpperType;

	// 右键预动作
	EUpperBody::Type RightUpperType;

	// 是否按住左右鼠标按键
	bool IsLeftButtonDown;
	bool IsRightButtonDown;

	// 检测到的资源
	AActor* RayActor;

	// 保存当前的UI状态
	EGameUIType::Type CurrentUIType;

	// 小地图缩放状态
	EMiniMapSizeMode::Type MiniMapSizeMode;

};
