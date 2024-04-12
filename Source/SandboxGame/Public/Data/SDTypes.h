// ��������Ŀ�л��õ��ĸ�������

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM()
enum class ECultureTeam : uint8 {
	EN = 0,
	ZH = 1
};

// Menu��ť������
namespace EMenuItem 
{
	enum Type{
		None,
		StartGame,
		GameOption,
		QuitGame,
		NewGame,
		LoadRecord,
		StartGameGoBack,
		GameOptionGoBack,
		NewGameGoBack,
		ChooseRecordGoBack,
		EnterGame,
		EnterRecord
	};
}

// Menu��������
namespace EMenuType
{
	enum Type
	{
		None,
		MainMenu,
		StartGame,
		GameOption,
		NewGame,
		ChooseRecord
	};
}

// Menu����״̬ö��
namespace EMenuAnim
{
	enum Type
	{
		Stop,	// ֹͣ����
		Close,  // �ر�Menu
		Open	// ��Menu
	};
}