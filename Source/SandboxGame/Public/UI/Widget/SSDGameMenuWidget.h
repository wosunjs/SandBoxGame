// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

// 存档委托
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

	// 游戏失败调用
	void GameLose();

	// 重置菜单
	void ResetMenu();

public:
	// 存档委托,绑定GameMode的SaveGame函数
	FSaveGameDelegate SaveGameDele;

private:
	void InitializeWidget();

	FReply OptionEvent();	// 设置
	FReply SaveGameEvent();	// 进入游戏
	FReply QuitGameEvent();	// 退出游戏
	FReply GoBackEvent();	// 返回主菜单

	void ChangeCulture(ECultureTeam Culture);	// 修改语言
	void ChangeVolume(const float MusicVolume, const float SoundVolume);	// 修改音量

private:
	// 获取GameWidgetStyle
	const FSDGameStyle* GameStyle;

	TSharedPtr<SBox> RootBox;	// 普通界面+设置界面+退出界面

	TSharedPtr<SVerticalBox> VertBox;	// 按钮和设置菜单

	TSharedPtr<SButton> SaveGameButton;		// 保存游戏按钮

	TSharedPtr<STextBlock> SaveGameText;	// 保存游戏按钮显示

	TSharedPtr<SButton> QuitGameButton;		// 退出游戏按钮

	TArray<TSharedPtr<SCompoundWidget>> MenuItemList;

	TArray<TSharedPtr<SCompoundWidget>> OptionItemList;
};
