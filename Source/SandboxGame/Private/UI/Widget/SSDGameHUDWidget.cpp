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
	// ��UIScaler�󶨵�������̬�޸�
	UIScaler.Bind(this, &SSDGameHUDWidget::GetUIScaler);

	ChildSlot
	[
		SNew(SDPIScaler)
			.DPIScale(UIScaler)
			[
				SNew(SOverlay)
					// �����
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Bottom)
					[
						SAssignNew(ShortcutWidget, SSDShortcutWidget)
					]
					
					// ���߼����Ϣ
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					[
						SAssignNew(RayInfoWidget, SSDRayInfoWidget)
					]

					// ׼��
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(PointerWidget, SSDPointerWidget)
					]

					// ���״̬
					+SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Top)
					[
						SAssignNew(PlayerStateWidget, SSDPlayerStateWidget)
					]

					// С��ͼ
					+SOverlay::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Top)
					[
						SAssignNew(MiniMapWidget, SSDMiniMapWidget)
					]

					// ������ʾ��
					+SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(20.f, 0.f, 0.f, 15.f))
					[
						SAssignNew(ChatShowWidget, SSDChatShowWidget)
					]

					// ����ɫ���֣������¼��������ϷUI�м�
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SAssignNew(BlackShade, SBorder)
							// ����Ϊ��ɫ͸��
							.ColorAndOpacity(TAttribute<FLinearColor>(FLinearColor(0.2f, 0.2f, 0.2f, 0.5f)))
							// ��ʼʱ����ʾ
							.Visibility(EVisibility::Hidden)
							[
								SNew(SImage)
							]
					]

					// ��Ϸ�˵�
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(GameMenuWidget, SSDGameMenuWidget)
							.Visibility(EVisibility::Hidden)
					]

					// ������
					+SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Bottom)
					[
						SAssignNew(ChatRoomWidget, SSDChatRoomWidget)
							.Visibility(EVisibility::Hidden)
					]

					// ����
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SAssignNew(PackageWidget, SSDPackageWidget)
							// ����DPI����
							.UIScaler(UIScaler)
							.Visibility(EVisibility::Hidden)
					]
			]
	];

	// �����ʼ��
	InitUIMap();
}

void SSDGameHUDWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// ÿ�������һ����Ϣ
	if (MessageTimeCount < 5.f) {
		MessageTimeCount += InDeltaTime;
	}
	else {
		// ���½ǲ���һ����Ϣ
		ChatShowWidget->AddMessage(NSLOCTEXT("SDGame", "Enemy", "Enemy"), NSLOCTEXT("SDGame", "EnemyDialogue", ":Fight with me !"));
		// �������ڲ���һ����Ϣ
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
	// ���ǰһ��ģʽ��Game������ʾ�ڰ�
	if (PreUI == EGameUIType::Game) {
		BlackShade->SetVisibility(EVisibility::Visible);
	}
	else {
		// ���ص�ǰ������ʾ��UI
		UIMap.Find(PreUI)->Get()->SetVisibility(EVisibility::Hidden);
	}
	// �����һ��ģʽ��Game�����غڰ�
	if (NextUI == EGameUIType::Game) {
		BlackShade->SetVisibility(EVisibility::Hidden);
	}
	else {
		// ��ʾ����״̬��Ӧ��UI
		UIMap.Find(NextUI)->Get()->SetVisibility(EVisibility::Visible);
		// �����һ��UI��������,�򻬶����ײ�
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

	// �����ί��
	ChatRoomWidget->PushMessage.BindRaw(ChatShowWidget.Get(), &SSDChatShowWidget::AddMessage);

	// ��Ϣ��ʱ����ʼΪ0
	MessageTimeCount = 0.f;


}
