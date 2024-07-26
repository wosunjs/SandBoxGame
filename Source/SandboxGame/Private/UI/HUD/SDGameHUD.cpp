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
	// ����ӿڴ���
	if (GEngine && GEngine->GameViewport) {
		// ����HUD�ؼ�
		SAssignNew(GameHUDWidget, SSDGameHUDWidget);
		// ��ӵ��ӿ�
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameHUDWidget.ToSharedRef()));
	}


}

void ASDGameHUD::BeginPlay()
{
	// �ȵ��ø���BeginPlay()��Controller��Character
	Super::BeginPlay();

	GM = Cast<ASDGameMode>(UGameplayStatics::GetGameMode(GWorld));
	
	if (!GM) {
		return;
	}
	// ȷ�������������ʼ��
	GM->InitGamePlayModule();

	// ��ע����������
	GameHUDWidget->ShortcutWidget->RegisterShortcutContainer.BindUObject(GM->SPState,
		&ASDPlayerState::RegisterShortcutContainer);

	// �����߼����Ϣ��ʾ
	GameHUDWidget->RayInfoWidget->RegisterRayInfoEvent.BindUObject(GM->SPState, 
		&ASDPlayerState::RegisterRayInfoEvent);

	// ���޸�׼��ί��
	GM->SPController->UpdatePointer.BindRaw(GameHUDWidget->PointerWidget.Get(), &SSDPointerWidget::UpdatePointer);

	// �󶨸������״̬��ί��
	GM->SPState->UpdateStateWidget.BindRaw(GameHUDWidget->PlayerStateWidget.Get(), &SSDPlayerStateWidget::UpdateStateWidget);

	// ����ʾUI��ί��
	GM->SPController->ShowGameUI.BindRaw(GameHUDWidget.Get(), &SSDGameHUDWidget::ShowGameUI);

	// ��ʼ���������������������
	GM->InitPackageManager.BindRaw(GameHUDWidget->PackageWidget.Get(), &SSDPackageWidget::InitPackageManager);

	// ��ע��С��ͼ��ͼί��
	GM->RegisterMiniMap.BindRaw(GameHUDWidget->MiniMapWidget.Get(), &SSDMiniMapWidget::RegisterMiniMap);

	// ��С��ͼ���ݸ���ί��
	GM->UpdateMapData.BindRaw(GameHUDWidget->MiniMapWidget.Get(), &SSDMiniMapWidget::UpdateMapData);
}
