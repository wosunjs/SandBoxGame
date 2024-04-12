// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SDMenuHUD.h"
#include "UI/Widget/SSDMenuHUDWidget.h"
#include "SlateBasics.h"

ASDMenuHUD::ASDMenuHUD()
{
	// 如果视口存在
	if (GEngine && GEngine->GameViewport) {
		// 创建Widget
		SAssignNew(MenuHUDWidget, SSDMenuHUDWidget);
		// 添加到视口，通过SNew(XXX)创建SlateWidget，返回一个共享指针 (SAssignNew(指针,XXX)便不返回指针)
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MenuHUDWidget.ToSharedRef()));


	}
}
