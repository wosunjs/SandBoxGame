// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

// �浵ί��
DECLARE_DELEGATE(FSaveGameDelegate);

/**
 * 
 */
class SANDBOXGAME_API SSDGameMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDGameMenuWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// ��Ϸʧ�ܵ���
	void GameLose();

	// ���ò˵�
	void ResetMenu();

public:
	// �浵ί��,��GameMode��SaveGame����
	FSaveGameDelegate SaveGameDele;

private:
	void InitializeWidget();

	FReply OptionEvent();	// ����
	FReply SaveGameEvent();	// ������Ϸ
	FReply QuitGameEvent();	// �˳���Ϸ
	FReply GoBackEvent();	// �������˵�

	void ChangeCulture(ECultureTeam Culture);	// �޸�����
	void ChangeVolume(const float MusicVolume, const float SoundVolume);	// �޸�����

private:
	// ��ȡGameWidgetStyle
	const FSDGameStyle* GameStyle;

	TSharedPtr<SBox> RootBox;	// ��ͨ����+���ý���+�˳�����

	TSharedPtr<SVerticalBox> VertBox;	// ��ť�����ò˵�

	TSharedPtr<SButton> SaveGameButton;		// ������Ϸ��ť

	TSharedPtr<STextBlock> SaveGameText;	// ������Ϸ��ť��ʾ

	TSharedPtr<SButton> QuitGameButton;		// �˳���Ϸ��ť

	TArray<TSharedPtr<SCompoundWidget>> MenuItemList;

	TArray<TSharedPtr<SCompoundWidget>> OptionItemList;
};
