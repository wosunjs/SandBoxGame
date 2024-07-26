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

	// �󶨵�UIScaler�ķ���
	float GetUIScaler() const;

	// ��ʾ��ϷUI����PlayerController��ShowGameUIί�а�
	void ShowGameUI(EGameUIType::Type PreUI, EGameUIType::Type NextUI);


public:
	// �����ָ��
	TSharedPtr<SSDShortcutWidget> ShortcutWidget;

	// ������Ϣ��
	TSharedPtr<SSDRayInfoWidget> RayInfoWidget;

	// ׼��
	TSharedPtr<SSDPointerWidget> PointerWidget;

	// ���״̬
	TSharedPtr<SSDPlayerStateWidget> PlayerStateWidget;

	// С��ͼ����
	TSharedPtr<SSDMiniMapWidget> MiniMapWidget;

	// ������ʾ������
	TSharedPtr<SSDChatShowWidget> ChatShowWidget;

	// ��Ϸ�˵�
	TSharedPtr<SSDGameMenuWidget> GameMenuWidget;

	// ������
	TSharedPtr<SSDChatRoomWidget> ChatRoomWidget;

	// ����
	TSharedPtr<SSDPackageWidget> PackageWidget;

private:
	// ��ȡ��ĻSize
	FVector2D GetViewportSize() const;

	// ��UI��ӴﵽUIMap
	void InitUIMap();

private:
	// DPI����
	TAttribute<float> UIScaler;

	// ��ɫ����
	TSharedPtr<SBorder> BlackShade;

	// UIMap
	TMap<EGameUIType::Type, TSharedPtr<SCompoundWidget>> UIMap;

	// ��Ϣ��ʱ��
	float MessageTimeCount;

};
