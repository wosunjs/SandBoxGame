// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDGameHUDWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/SOverlay.h"

#include "UI/Widget/SSDShortcutWidget.h"
#include "UI/Widget/SSDRayInfoWidget.h"
#include "UI/Widget/SSDPointerWidget.h"
#include "UI/Widget/SSDPlayerStateWidget.h"
#include "UI/Widget/SSDGameMenuWidget.h"
#include "UI/Widget/SSDChatRoomWidget.h"
#include "UI/Widget/Package/SSDPackageWidget.h"
#include "UI/Widget/SSDMiniMapWidget.h"
#include "UI/Widget/SSDChatShowWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDGameHUDWidget::Construct(const FArguments& InArgs)
{
	// 将UIScaler绑定到函数动态修改
	UIScaler.Bind(this, &SSDGameHUDWidget::GetUIScaler);

	ChildSlot
	[
		SNew(SDPIScaler)
			.DPIScale(UIScaler)
			[
				SNew(SOverlay)
					// 快捷栏
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Bottom)
					[
						SAssignNew(ShortcutWidget, SSDShortcutWidget)
					]
					
					// 射线检测信息
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					[
						SAssignNew(RayInfoWidget, SSDRayInfoWidget)
					]

					// 准星
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(PointerWidget, SSDPointerWidget)
					]

					// 玩家状态
					+SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Top)
					[
						SAssignNew(PlayerStateWidget, SSDPlayerStateWidget)
					]

					// 小地图
					+SOverlay::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Top)
					[
						SAssignNew(MiniMapWidget, SSDMiniMapWidget)
					]

					// 聊天显示栏
					+SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(20.f, 0.f, 0.f, 15.f))
					[
						SAssignNew(ChatShowWidget, SSDChatShowWidget)
					]

					// 暗黑色遮罩，放在事件界面和游戏UI中间
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SAssignNew(BlackShade, SBorder)
							// 设置为黑色透明
							.ColorAndOpacity(TAttribute<FLinearColor>(FLinearColor(0.2f, 0.2f, 0.2f, 0.5f)))
							// 开始时不显示
							.Visibility(EVisibility::Hidden)
							[
								SNew(SImage)
							]
					]

					// 游戏菜单
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(GameMenuWidget, SSDGameMenuWidget)
							.Visibility(EVisibility::Hidden)
					]

					// 聊天室
					+SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Bottom)
					[
						SAssignNew(ChatRoomWidget, SSDChatRoomWidget)
							.Visibility(EVisibility::Hidden)
					]

					// 背包
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SAssignNew(PackageWidget, SSDPackageWidget)
							// 传入DPI参数
							.UIScaler(UIScaler)
							.Visibility(EVisibility::Hidden)
					]
			]
	];

	// 其余初始化
	InitUIMap();
}

void SSDGameHUDWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// 每五秒插入一条消息
	if (MessageTimeCount < 5.f) {
		MessageTimeCount += InDeltaTime;
	}
	else {
		// 左下角插入一条信息
		ChatShowWidget->AddMessage(NSLOCTEXT("SDGame", "Enemy", "Enemy"), NSLOCTEXT("SDGame", "EnemyDialogue", ":Fight with me !"));
		// 聊天室内插入一条信息
		ChatRoomWidget->AddMessage(NSLOCTEXT("SDGame", "Enemy", "Enemy"), NSLOCTEXT("SDGame", "EnemyDialogue", ":Fight with me !"));

		MessageTimeCount = 0.f;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

float SSDGameHUDWidget::GetUIScaler() const
{
	return GetViewportSize().Y / 1080.f;
}

void SSDGameHUDWidget::ShowGameUI(EGameUIType::Type PreUI, EGameUIType::Type NextUI)
{
	// 如果前一个模式是Game，则显示黑板
	if (PreUI == EGameUIType::Game) {
		BlackShade->SetVisibility(EVisibility::Visible);
	}
	else {
		// 隐藏当前正在显示的UI
		UIMap.Find(PreUI)->Get()->SetVisibility(EVisibility::Hidden);
	}
	// 如果下一个模式是Game，隐藏黑板
	if (NextUI == EGameUIType::Game) {
		BlackShade->SetVisibility(EVisibility::Hidden);
	}
	else {
		// 显示现在状态对应的UI
		UIMap.Find(NextUI)->Get()->SetVisibility(EVisibility::Visible);
		// 如果下一个UI是聊天室,则滑动到底部
		if (NextUI == EGameUIType::ChatRoom) {
			ChatRoomWidget->ScrollToEnd();
		}
		if (NextUI == EGameUIType::Lose) {
			GameMenuWidget->GameLose();
		}
		if (NextUI == EGameUIType::Pause) {
			GameMenuWidget->ResetMenu();
		}
	}
}

FVector2D SSDGameHUDWidget::GetViewportSize() const
{
	FVector2D Result(1920.f, 1080.f);
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(Result);
	}
	return Result;
}

void SSDGameHUDWidget::InitUIMap()
{
	UIMap.Add(EGameUIType::Pause, GameMenuWidget);
	UIMap.Add(EGameUIType::Package, PackageWidget);
	UIMap.Add(EGameUIType::ChatRoom, ChatRoomWidget);
	UIMap.Add(EGameUIType::Lose, GameMenuWidget);

	// 绑定相关委托
	ChatRoomWidget->PushMessage.BindRaw(ChatShowWidget.Get(), &SSDChatShowWidget::AddMessage);

	// 消息计时器初始为0
	MessageTimeCount = 0.f;


}
