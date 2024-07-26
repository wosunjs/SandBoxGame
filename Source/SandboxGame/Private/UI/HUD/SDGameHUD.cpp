// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SDGameHUD.h"
#include "UI/Widget/SSDGameHUDWidget.h"
#include "UI/Widget/SSDShortcutWidget.h"
#include "UI/Widget/SSDRayInfoWidget.h"
#include "UI/Widget/SSDPointerWidget.h"
#include "UI/Widget/SSDPlayerStateWidget.h"
#include "UI/Widget/Package/SSDPackageWidget.h"
#include "UI/Widget/SSDMiniMapWidget.h"

#include "Player/SDPlayerCharacter.h"
#include "Player/SDPlayerController.h"
#include "Player/SDPlayerState.h"
#include "GamePlay/SDGameMode.h"

#include "SlateBasics.h"
#include "Kismet/GameplayStatics.h"

ASDGameHUD::ASDGameHUD()
{
	// 如果视口存在
	if (GEngine && GEngine->GameViewport) {
		// 创建HUD控件
		SAssignNew(GameHUDWidget, SSDGameHUDWidget);
		// 添加到视口
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameHUDWidget.ToSharedRef()));
	}


}

void ASDGameHUD::BeginPlay()
{
	// 先调用父类BeginPlay()绑定Controller和Character
	Super::BeginPlay();

	GM = Cast<ASDGameMode>(UGameplayStatics::GetGameMode(GWorld));
	
	if (!GM) {
		return;
	}
	// 确保所需组件都初始化
	GM->InitGamePlayModule();

	// 绑定注册快捷栏容器
	GameHUDWidget->ShortcutWidget->RegisterShortcutContainer.BindUObject(GM->SPState,
		&ASDPlayerState::RegisterShortcutContainer);

	// 绑定射线检测信息显示
	GameHUDWidget->RayInfoWidget->RegisterRayInfoEvent.BindUObject(GM->SPState, 
		&ASDPlayerState::RegisterRayInfoEvent);

	// 绑定修改准星委托
	GM->SPController->UpdatePointer.BindRaw(GameHUDWidget->PointerWidget.Get(), &SSDPointerWidget::UpdatePointer);

	// 绑定更新玩家状态的委托
	GM->SPState->UpdateStateWidget.BindRaw(GameHUDWidget->PlayerStateWidget.Get(), &SSDPlayerStateWidget::UpdateStateWidget);

	// 绑定显示UI的委托
	GM->SPController->ShowGameUI.BindRaw(GameHUDWidget.Get(), &SSDGameHUDWidget::ShowGameUI);

	// 初始化背包管理器到背包组件
	GM->InitPackageManager.BindRaw(GameHUDWidget->PackageWidget.Get(), &SSDPackageWidget::InitPackageManager);

	// 绑定注册小地图贴图委托
	GM->RegisterMiniMap.BindRaw(GameHUDWidget->MiniMapWidget.Get(), &SSDMiniMapWidget::RegisterMiniMap);

	// 绑定小地图数据更新委托
	GM->UpdateMapData.BindRaw(GameHUDWidget->MiniMapWidget.Get(), &SSDMiniMapWidget::UpdateMapData);
}
