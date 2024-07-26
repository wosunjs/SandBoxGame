// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SANDBOXGAME_API SDInternation
{
public:
	// ��ʱ�������ṩ���ػ����ֵĽӿ�
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

SDInternation::Register(LOCTEXT("RecordName", "RecordName"));	// �浵��
SDInternation::Register(LOCTEXT("EnterRecord", "EnterRecord"));
SDInternation::Register(LOCTEXT("RecordNameHint", "Input Record Name!"));				// ����浵��
SDInternation::Register(LOCTEXT("RecordNameRepeateHint", "Record Name Repeated!"));		// �浵���ظ�

SDInternation::Register(LOCTEXT("Chinese", "Chinese"));
SDInternation::Register(LOCTEXT("English", "English"));

SDInternation::Register(LOCTEXT("Music", "Music"));
SDInternation::Register(LOCTEXT("Sound", "Sound"));

SDInternation::Register(LOCTEXT("GoBack", "GoBack"));

#undef LOCTEXT_NAMESPACE


#define LOCTEXT_NAMESPACE "SDGame"

SDInternation::Register(LOCTEXT("E", "E"));	// ��
SDInternation::Register(LOCTEXT("S", "S")); // ��
SDInternation::Register(LOCTEXT("W", "W")); // ��
SDInternation::Register(LOCTEXT("N", "N")); // ��

SDInternation::Register(LOCTEXT("Player", "Player"));	// ���
SDInternation::Register(LOCTEXT("Enemy", "Enemy"));		// ����

SDInternation::Register(LOCTEXT("EnemyDialogue", ": Fight with me!"));	// ���˶԰�
SDInternation::Register(LOCTEXT("Send", "Send"));		// ����

SDInternation::Register(LOCTEXT("GameOption", "GameOption"));	// ��Ϸ����
SDInternation::Register(LOCTEXT("SaveGame", "SaveGame"));		// ������Ϸ
SDInternation::Register(LOCTEXT("SaveCompleted", "SaveCompleted"));		// ����ɹ�
SDInternation::Register(LOCTEXT("QuitGame", "QuitGame"));		// �˳���Ϸ
SDInternation::Register(LOCTEXT("GoBack", "GoBack"));	// ����

#undef LOCTEXT_NAMESPACE