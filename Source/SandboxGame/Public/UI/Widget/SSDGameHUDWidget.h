// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SSDShortcutWidget;
class SSDRayInfoWidget;
class SSDPointerWidget;
class SSDPlayerStateWidget;
class SSDGameMenuWidget;
class SSDChatRoomWidget;
class SSDPackageWidget;
class SSDMiniMapWidget;
class SSDChatShowWidget;

/**
 * 
 */
class SANDBOXGAME_API SSDGameHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDGameHUDWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// 绑定到UIScaler的方法
	float GetUIScaler() const;

	// 显示游戏UI，被PlayerController的ShowGameUI委托绑定
	void ShowGameUI(EGameUIType::Type PreUI, EGameUIType::Type NextUI);


public:
	// 快捷栏指针
	TSharedPtr<SSDShortcutWidget> ShortcutWidget;

	// 射线信息框
	TSharedPtr<SSDRayInfoWidget> RayInfoWidget;

	// 准星
	TSharedPtr<SSDPointerWidget> PointerWidget;

	// 玩家状态
	TSharedPtr<SSDPlayerStateWidget> PlayerStateWidget;

	// 小地图引用
	TSharedPtr<SSDMiniMapWidget> MiniMapWidget;

	// 聊天显示栏引用
	TSharedPtr<SSDChatShowWidget> ChatShowWidget;

	// 游戏菜单
	TSharedPtr<SSDGameMenuWidget> GameMenuWidget;

	// 聊天室
	TSharedPtr<SSDChatRoomWidget> ChatRoomWidget;

	// 背包
	TSharedPtr<SSDPackageWidget> PackageWidget;

private:
	// 获取屏幕Size
	FVector2D GetViewportSize() const;

	// 将UI添加达到UIMap
	void InitUIMap();

private:
	// DPI缩放
	TAttribute<float> UIScaler;

	// 黑色遮罩
	TSharedPtr<SBorder> BlackShade;

	// UIMap
	TMap<EGameUIType::Type, TSharedPtr<SCompoundWidget>> UIMap;

	// 消息计时器
	float MessageTimeCount;

};
