// ��������Ŀ�л��õ��ĸ�������

#pragma once

#include "CoreMinimal.h"
#include "Common/SDHelper.h"

/**
 * 
 */

UENUM()
enum class ECultureTeam : uint8 
{
	EN = 0,
	ZH = 1
};

// Menu��ť������
namespace EMenuItem 
{
	enum Type
	{
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

// �ӽ�״̬
namespace EGameViewMode 
{
	enum Type
	{
		First,
		Third
	};
}

// �ϰ�������״̬
namespace EUpperBody
{
	enum Type
	{
		None,
		Punch,
		Hit,
		Fight,
		PickUp,
		Eat
	};
}

// ��Ʒ����
namespace EObjectType
{
	enum Type
	{
		Normal = 0,	// ��ͨ��Ʒ��ľͷ��ʯͷ
		Food,		// ʳ�ƻ������
		Tool,		// ���ߣ����ӡ�����
		Weapon		// ��������
	};
}

// ��Ʒ���Խṹ��
struct ObjectAttribute 
{
	FText EN;	// Ӣ����
	FText ZH;	// ������
	EObjectType::Type ObjectType;	// ��Ʒ����
	int PlantAttack;	// ��ֲ��Ĺ�����
	int MetalAttack;	// �Խ�����Դ�Ĺ�����
	int AnimalAttack;	// �Զ���Ĺ�����
	int AffectRange;	// ��������
	FString TexPath;	// ͼƬ·��

	// ���캯��
	ObjectAttribute(const FText ENName, const FText ZHName, const EObjectType::Type OT, const int PA, const int MA,
		const int AA, const int AR, const FString TP) : EN(ENName), ZH(ZHName), ObjectType(OT), PlantAttack(PA),
		MetalAttack(MA), AnimalAttack(MA), AffectRange(AR), TexPath(TP)
	{}

};

// ����������ṹ��
struct ShortcutContainer
{
	int ObjectIndex;	// ��ƷID
	int ObjectNum;		// ��Ʒ����
	TSharedPtr<SBorder> ContainerBorder;
	TSharedPtr<SBorder> ObjectImage;
	TSharedPtr<STextBlock> ObjectNumText;
	const FSlateBrush* NormalContainerBrush;
	const FSlateBrush* ChoosedContainerBrush;
	TArray<const FSlateBrush*>* ObjectBrushList;

	// ��ʼ�����캯��
	ShortcutContainer(TSharedPtr<SBorder> CB, TSharedPtr<SBorder> OI, TSharedPtr<STextBlock> ONT, 
		const FSlateBrush* NCB, const FSlateBrush* CCB, TArray<const FSlateBrush*>* OBL) : 
		ContainerBorder(CB), ObjectImage(OI), ObjectNumText(ONT), NormalContainerBrush(NCB),
		ChoosedContainerBrush(CCB), ObjectBrushList(OBL){
		// ��ʼ����ʾ����
		ObjectIndex = 0;
		ObjectNum = 0;
		ContainerBorder->SetBorderImage(NormalContainerBrush);
		ObjectImage->SetBorderImage((*ObjectBrushList)[0]);
	}

	// �����Ƿ�ѡ�е�ǰ��Ʒ
	int SetChoosed(bool Option) {
		if (Option) {
			ContainerBorder->SetBorderImage(ChoosedContainerBrush);
		}
		else {
			ContainerBorder->SetBorderImage(NormalContainerBrush);
		}
		return ObjectIndex;
	}

	// ��Ʒ�����ðڷŵ���Ʒ��������Ʒid
	ShortcutContainer* SetObject(int NewIndex) {
		ObjectIndex = NewIndex;
		ObjectImage->SetBorderImage((*ObjectBrushList)[ObjectIndex]);
		return this;
	}

	// ����������������ʾ
	ShortcutContainer* SetObjectNum(int Num = 0) {
		ObjectNum = Num;
		// �����Ʒ����Ϊ0or1ʱ������ʾ����
		if (ObjectNum == 0 || ObjectNum == 1) {
			ObjectNumText->SetText(FString(""));
		}
		else {
			ObjectNumText->SetText(FString::FromInt(ObjectNum));
		}
		return this;
	}

};


// ��Դ����
namespace EResourceType {
	enum Type
	{
		Plant = 0,
		Metal,
		Animal
	};
}


// ��Դ���Խṹ��
struct ResourceAttribute
{
	FText EN;	// Ӣ����
	FText ZH;	// ������
	EResourceType::Type ResourceType;
	int HP;
	TArray<TArray<int>> FlobObjectInfo;		// ����������

	ResourceAttribute(const FText ENName, const FText ZHName, const EResourceType::Type RT
		, const int HPValue, TArray<TArray<int>>* FOI) {
		EN = ENName;
		ZH = ZHName;
		ResourceType = RT;
		HP = HPValue;
		
		// ������Ԫ�ص�������������
		for (TArray<TArray<int>>::TIterator It(*FOI); It; ++It) {
			TArray<int> FlobObjectInfoItem;
			for (TArray<int>::TIterator Ih(*It); Ih; ++Ih) {
				FlobObjectInfoItem.Add(*Ih);
			}
			FlobObjectInfo.Add(FlobObjectInfoItem);
		}
	}

	// Test Code
	FString ToString() {
		FString InfoStr;
		for (TArray<TArray<int>>::TIterator It(FlobObjectInfo); It; ++It) {
			for (TArray<int>::TIterator Ih(*It); Ih; ++Ih) {
				InfoStr += FString::FromInt(*Ih) + FString(".");
			}
			InfoStr += FString("__");
		}
		return EN.ToString() + FString("--") + ZH.ToString() + FString("--") + FString::FromInt
		((int)ResourceType) + FString("--") + FString::FromInt(HP) + FString("--") + InfoStr;
	}
};

// Game�������
namespace EGameUIType
{
	enum Type
	{
		Game,	// ��ϷģʽUI
		Pause,	// ��ͣ
		Lose,	// ����
		Package,	// ����
		ChatRoom	// ������
	};
}

// ������������
namespace EContainerType 
{
	enum Type 
	{
		Output,		// �ϳ��������
		Input,		// �ϳ��������
		Normal,		// ��ͨ����
		Shortcut	// ���������
	};
}

// �ϳɱ�ṹ��
struct CompoundTable
{
	// �ϳ�ͼ
	TArray<int> CompoundMap;
	// ���캯��
	CompoundTable(TArray<int>* InsertMap) {
		for (TArray<int>::TIterator It(*InsertMap); It; ++It) {
			CompoundMap.Add(*It);
		}
	}

	// �����Ϻϳɱ�������ƷID������
	void DetectTable(TArray<int>* IDMap, TArray<int>* NumMap, int& OutputID, int& OutputNum) {
		// ��ȡ�úϳɱ����ID
		int TempID = CompoundMap[9];
		// Ĭ���������Ϊ64
		int TempNum = 64;
		for (int i = 0; i < 9; ++i) {
			// ���������Ʒ�ͷ�����Ʒ��ͬ,�ҷǿ�����½������
			if ((*IDMap)[i] == CompoundMap[i]) {
				if (CompoundMap[i] != 0)TempNum = FMath::Min(TempNum, (*NumMap)[i]);
			}
			else {
				TempID = TempNum = 0;
				break;
			}
		}
		// ��������Ϊ�գ������Output����
		if (TempID && TempNum) {
			OutputID = TempID;
			OutputNum= TempNum;
		}
	}

	// ���ݺϳ�̨ƥ���Ӧ�ϳɱ��۳��ϳ�̨���ɵ���Ӧ��������ProductNum����ΪExpendMap
	bool DetectExpend(TArray<int>* TableMap, int ProductNum, TArray<int>& ExpendMap) {
		// ƥ����
		bool IsMathch = true;
		// ���ϳ�̨�����Ƿ���ñ�ƥ��
		for (int i = 0; i < 10; i++) {
			if ((*TableMap)[i] != CompoundMap[i]) {
				IsMathch = false;
				break;
			}
		}
		// ���ƥ��
		if (IsMathch) {
			for (int i = 0; i < 9; ++i) {
				// ��������Ϊ�գ��۳���Ӧ����
				if (CompoundMap[i] != 0) {
					ExpendMap.Add(ProductNum);
				}
				else {
					ExpendMap.Add(0);
				}
			}
		}
		return IsMathch;
	}

	// ��ӡ�ַ�
	FString ToString() {
		FString OutputString("");
		for (TArray<int>::TIterator It(CompoundMap); It; ++It) {
			OutputString += FString::FromInt(*It) + FString("__");
		}
		return OutputString;
	}
};

// ����AI״̬
UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	ES_Patrol	UMETA(DisplayName = "Patrol"),
	ES_Chase	UMETA(DisplayName = "Chase"),
	ES_Escape	UMETA(DisplayName = "Escape"),
	ES_Attack	UMETA(DisplayName = "Attack"),
	ES_Hurt		UMETA(DisplayName = "Hurt"),
	ES_Defence	UMETA(DisplayName = "Defence"),
	ES_Dead		UMETA(DisplayName = "Dead")
};

// ���˹���״̬
UENUM(BlueprintType)
enum class EEnemyAttakType : uint8
{
	EA_Normal	UMETA(DisplayName = "Normal"),
	EA_Dash		UMETA(DisplayName = "Dash"),
	EA_Pursuit	UMETA(DisplayName = "Pursuit")
};

// С��ͼ����״̬
namespace EMiniMapSizeMode
{
	enum Type
	{
		None,
		Add,
		Reduce
	};
}