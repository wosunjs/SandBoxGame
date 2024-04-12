// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/SDMenuController.h"

ASDMenuController::ASDMenuController()
{
	bShowMouseCursor = true;	// 显示鼠标
}

void ASDMenuController::BeginPlay()
{
	// 设置输入模式
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);	// 让鼠标一直锁在输入框内
	SetInputMode(InputMode);	// 绑定到Controller

}
