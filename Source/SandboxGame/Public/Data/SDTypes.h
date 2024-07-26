// 定义在项目中会用到的各种类型

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

// Menu按钮的类型
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

// Menu界面类型
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

// Menu动画状态枚举
namespace EMenuAnim
{
	enum Type
	{
		Stop,	// 停止动画
		Close,  // 关闭Menu
		Open	// 打开Menu
	};
}

// 视角状态
namespace EGameViewMode 
{
	enum Type
	{
		First,
		Third
	};
}

// 上半身动画的状态
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

// 物品类型
namespace EObjectType
{
	enum Type
	{
		Normal = 0,	// 普通物品：木头、石头
		Food,		// 食物：苹果、肉
		Tool,		// 工具：锤子、斧子
		Weapon		// 武器：剑
	};
}

// 物品属性结构体
struct ObjectAttribute 
{
	FText EN;	// 英文名
	FText ZH;	// 中文名
	EObjectType::Type ObjectType;	// 物品类型
	int PlantAttack;	// 对植物的攻击力
	int MetalAttack;	// 对金属资源的攻击力
	int AnimalAttack;	// 对动物的攻击力
	int AffectRange;	// 攻击距离
	FString TexPath;	// 图片路径

	// 构造函数
	ObjectAttribute(const FText ENName, const FText ZHName, const EObjectType::Type OT, const int PA, const int MA,
		const int AA, const int AR, const FString TP) : EN(ENName), ZH(ZHName), ObjectType(OT), PlantAttack(PA),
		MetalAttack(MA), AnimalAttack(MA), AffectRange(AR), TexPath(TP)
	{}

};

// 快捷栏容器结构体
struct ShortcutContainer
{
	int ObjectIndex;	// 物品ID
	int ObjectNum;		// 物品数量
	TSharedPtr<SBorder> ContainerBorder;
	TSharedPtr<SBorder> ObjectImage;
	TSharedPtr<STextBlock> ObjectNumText;
	const FSlateBrush* NormalContainerBrush;
	const FSlateBrush* ChoosedContainerBrush;
	TArray<const FSlateBrush*>* ObjectBrushList;

	// 初始化构造函数
	ShortcutContainer(TSharedPtr<SBorder> CB, TSharedPtr<SBorder> OI, TSharedPtr<STextBlock> ONT, 
		const FSlateBrush* NCB, const FSlateBrush* CCB, TArray<const FSlateBrush*>* OBL) : 
		ContainerBorder(CB), ObjectImage(OI), ObjectNumText(ONT), NormalContainerBrush(NCB),
		ChoosedContainerBrush(CCB), ObjectBrushList(OBL){
		// 初始化显示设置
		ObjectIndex = 0;
		ObjectNum = 0;
		ContainerBorder->SetBorderImage(NormalContainerBrush);
		ObjectImage->SetBorderImage((*ObjectBrushList)[0]);
	}

	// 设置是否选中当前物品
	int SetChoosed(bool Option) {
		if (Option) {
			ContainerBorder->SetBorderImage(ChoosedContainerBrush);
		}
		else {
			ContainerBorder->SetBorderImage(NormalContainerBrush);
		}
		return ObjectIndex;
	}

	// 物品栏设置摆放的物品，传入物品id
	ShortcutContainer* SetObject(int NewIndex) {
		ObjectIndex = NewIndex;
		ObjectImage->SetBorderImage((*ObjectBrushList)[ObjectIndex]);
		return this;
	}

	// 设置数量及数字显示
	ShortcutContainer* SetObjectNum(int Num = 0) {
		ObjectNum = Num;
		// 如果物品数量为0or1时，不显示数字
		if (ObjectNum == 0 || ObjectNum == 1) {
			ObjectNumText->SetText(FString(""));
		}
		else {
			ObjectNumText->SetText(FString::FromInt(ObjectNum));
		}
		return this;
	}

};


// 资源类型
namespace EResourceType {
	enum Type
	{
		Plant = 0,
		Metal,
		Animal
	};
}


// 资源属性结构体
struct ResourceAttribute
{
	FText EN;	// 英文名
	FText ZH;	// 中文名
	EResourceType::Type ResourceType;
	int HP;
	TArray<TArray<int>> FlobObjectInfo;		// 掉落物数组

	ResourceAttribute(const FText ENName, const FText ZHName, const EResourceType::Type RT
		, const int HPValue, TArray<TArray<int>>* FOI) {
		EN = ENName;
		ZH = ZHName;
		ResourceType = RT;
		HP = HPValue;
		
		// 将数组元素迭代进本地数组
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

// Game界面分类
namespace EGameUIType
{
	enum Type
	{
		Game,	// 游戏模式UI
		Pause,	// 暂停
		Lose,	// 死亡
		Package,	// 背包
		ChatRoom	// 聊天室
	};
}

// 背包容器类型
namespace EContainerType 
{
	enum Type 
	{
		Output,		// 合成输出格子
		Input,		// 合成输入格子
		Normal,		// 普通格子
		Shortcut	// 快捷栏格子
	};
}

// 合成表结构体
struct CompoundTable
{
	// 合成图
	TArray<int> CompoundMap;
	// 构造函数
	CompoundTable(TArray<int>* InsertMap) {
		for (TArray<int>::TIterator It(*InsertMap); It; ++It) {
			CompoundMap.Add(*It);
		}
	}

	// 检测符合合成表的输出物品ID及数量
	void DetectTable(TArray<int>* IDMap, TArray<int>* NumMap, int& OutputID, int& OutputNum) {
		// 获取该合成表输出ID
		int TempID = CompoundMap[9];
		// 默认输出数量为64
		int TempNum = 64;
		for (int i = 0; i < 9; ++i) {
			// 如果所需物品和放置物品相同,且非空则更新结果数量
			if ((*IDMap)[i] == CompoundMap[i]) {
				if (CompoundMap[i] != 0)TempNum = FMath::Min(TempNum, (*NumMap)[i]);
			}
			else {
				TempID = TempNum = 0;
				break;
			}
		}
		// 如果输出不为空，则更新Output数据
		if (TempID && TempNum) {
			OutputID = TempID;
			OutputNum= TempNum;
		}
	}

	// 根据合成台匹配对应合成表，扣除合成台生成的相应物资数量ProductNum，作为ExpendMap
	bool DetectExpend(TArray<int>* TableMap, int ProductNum, TArray<int>& ExpendMap) {
		// 匹配结果
		bool IsMathch = true;
		// 检测合成台放置是否与该表匹配
		for (int i = 0; i < 10; i++) {
			if ((*TableMap)[i] != CompoundMap[i]) {
				IsMathch = false;
				break;
			}
		}
		// 如果匹配
		if (IsMathch) {
			for (int i = 0; i < 9; ++i) {
				// 若该栏不为空，扣除对应数量
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

	// 打印字符
	FString ToString() {
		FString OutputString("");
		for (TArray<int>::TIterator It(CompoundMap); It; ++It) {
			OutputString += FString::FromInt(*It) + FString("__");
		}
		return OutputString;
	}
};

// 敌人AI状态
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

// 敌人攻击状态
UENUM(BlueprintType)
enum class EEnemyAttakType : uint8
{
	EA_Normal	UMETA(DisplayName = "Normal"),
	EA_Dash		UMETA(DisplayName = "Dash"),
	EA_Pursuit	UMETA(DisplayName = "Pursuit")
};

// 小地图缩放状态
namespace EMiniMapSizeMode
{
	enum Type
	{
		None,
		Add,
		Reduce
	};
}