// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SDMenuHUD.h"
#include "UI/Widget/SSDMenuHUDWidget.h"
#include "SlateBasics.h"

ASDMenuHUD::ASDMenuHUD()
{
	// ����ӿڴ���
	if (GEngine && GEngine->GameViewport) {
		// ����Widget
		SAssignNew(MenuHUDWidget, SSDMenuHUDWidget);
		// ��ӵ��ӿڣ�ͨ��SNew(XXX)����SlateWidget������һ������ָ�� (SAssignNew(ָ��,XXX)�㲻����ָ��)
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MenuHUDWidget.ToSharedRef()));


	}
}
