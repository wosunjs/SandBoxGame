// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SANDBOXGAME_API SDInternation
{
public:
	// 临时函数，提供本地化文字的接口
	static void Register(FText Value) {
		return;
	}
};

#define LOCTEXT_NAMESPACE "SDMenu"

SDInternation::Register(LOCTEXT("Menu", "Menu"));

SDInternation::Register(LOCTEXT("StartGame", "StartGame"));
SDInternation::Register(LOCTEXT("EnterGame", "EnterGame"));
SDInternation::Register(LOCTEXT("GameOption", "GameOption"));
SDInternation::Register(LOCTEXT("QuitGame", "QuitGame"));

SDInternation::Register(LOCTEXT("NewGame", "NewGame"));
SDInternation::Register(LOCTEXT("LoadRecord", "LoadRecord"));
SDInternation::Register(LOCTEXT("ChooseRecord", "ChooseRecord"));

SDInternation::Register(LOCTEXT("RecordName", "RecordName"));	// 存档名
SDInternation::Register(LOCTEXT("EnterRecord", "EnterRecord"));
SDInternation::Register(LOCTEXT("RecordNameHint", "Input Record Name!"));				// 输入存档名
SDInternation::Register(LOCTEXT("RecordNameRepeateHint", "Record Name Repeated!"));		// 存档名重复

SDInternation::Register(LOCTEXT("Chinese", "Chinese"));
SDInternation::Register(LOCTEXT("English", "English"));

SDInternation::Register(LOCTEXT("Music", "Music"));
SDInternation::Register(LOCTEXT("Sound", "Sound"));

SDInternation::Register(LOCTEXT("GoBack", "GoBack"));

#undef LOCTEXT_NAMESPACE


#define LOCTEXT_NAMESPACE "SDGame"

SDInternation::Register(LOCTEXT("E", "E"));	// 东
SDInternation::Register(LOCTEXT("S", "S")); // 南
SDInternation::Register(LOCTEXT("W", "W")); // 西
SDInternation::Register(LOCTEXT("N", "N")); // 北

SDInternation::Register(LOCTEXT("Player", "Player"));	// 玩家
SDInternation::Register(LOCTEXT("Enemy", "Enemy"));		// 敌人

SDInternation::Register(LOCTEXT("EnemyDialogue", ": Fight with me!"));	// 敌人对白
SDInternation::Register(LOCTEXT("Send", "Send"));		// 发送

SDInternation::Register(LOCTEXT("GameOption", "GameOption"));	// 游戏设置
SDInternation::Register(LOCTEXT("SaveGame", "SaveGame"));		// 保存游戏
SDInternation::Register(LOCTEXT("SaveCompleted", "SaveCompleted"));		// 保存成功
SDInternation::Register(LOCTEXT("QuitGame", "QuitGame"));		// 退出游戏
SDInternation::Register(LOCTEXT("GoBack", "GoBack"));	// 返回

#undef LOCTEXT_NAMESPACE