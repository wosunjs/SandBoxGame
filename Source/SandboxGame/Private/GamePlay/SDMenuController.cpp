// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/SDMenuController.h"

ASDMenuController::ASDMenuController()
{
	bShowMouseCursor = true;	// ��ʾ���
}

void ASDMenuController::BeginPlay()
{
	// ��������ģʽ
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);	// �����һֱ�����������
	SetInputMode(InputMode);	// �󶨵�Controller

}
