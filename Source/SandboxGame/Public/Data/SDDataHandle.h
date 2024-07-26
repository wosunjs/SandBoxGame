// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SDTypes.h"

class USoundCue;
struct FSDMenuStyle;
struct FSDGameStyle;

/**
 * 
 */
class SANDBOXGAME_API SDDataHandle
{
public:
	SDDataHandle();

	static void Initialize();

	static TSharedPtr<SDDataHandle> Get();

	// 修改中英文
	void ChangeLocalizationCulture(ECultureTeam Culture);

	// 获取本地化
	ECultureTeam GetLocalizationCulture();

	// 修改音量 
	void ChangeVolume(float MusicValue, float SoundValue);

	// 游戏数据初始化
	void InitGameData();

public:
	// 音量
	float MusicVolume;

	// 音效
	float SoundVolume;

	// 存档数据
	TArray<FString> RecordDataList;

	// 当前存档名
	FString RecordName;

	// 物品属性集合
	TMap<int, TSharedPtr<ObjectAttribute>> ObjectAttrMap;

	// 物品贴图资源数组
	TArray<const FSlateBrush*> ObjectBrushList;

	// 资源属性集合
	TMap<int, TSharedPtr<ResourceAttribute>> ResourceAttrMap;

	// 合成表图
	TArray<TSharedPtr<CompoundTable>> CompoundTableMap;

protected:
	// 本地化语言
	ECultureTeam CurrentCulture;

private:
	// 单例创建
	static TSharedPtr<SDDataHandle> Create();

	// 获取存档数据进行初始化
	void InitRecordData();

	// 根据enum类型获取字符串(仅限UEnum才能通过值反射字符串)
	template<typename TEnum>
	FString GetEnumValueAsString(const FString& Name, TEnum Value);

	// 根据字符串获取Enum值
	template<typename TEnum>
	TEnum GetEnumValueFromString(const FString& Name, FString Value);

	// 初始化Menu声音数据
	void InitializeMenuAudio();

	// 初始化物品属性集合
	void InitObjectAttr();

	// 初始化资源属性图
	void InitResourceAttrMap();

	// 初始化合成表图
	void InitCompoundTableMap();

private:
	static TSharedPtr<SDDataHandle> DataInstance;	// 单例实例

	// 获取Menu样式
	const FSDMenuStyle* MenuStyle;

	// 获取Game样式
	const FSDGameStyle* GameStyle;

	// 保存Menu的声音资源
	TMap<FString, TArray<USoundCue*>> MenuAudioResource;

	//

};
