// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SDTypes.h"

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

	ECultureTeam GetLocalizationCulture();

	// 修改音量 
	void ChangeVolume(float MusicValue, float SoundValue);

public:
	// 音量
	float MusicVolume;

	// 音效
	float SoundVolume;

	// 存档数据
	TArray<FString> RecordDataList;

	// 当前存档名
	FString RecordName;

protected:
	// 本地化语言
	ECultureTeam CurrentCulture;

private:
	static TSharedPtr<SDDataHandle> Create();	// 单例创建

	// 获取存档数据进行初始化
	void InitRecordData();

	// 根据enum类型获取字符串(仅限UEnum才能通过值反射字符串)
	template<typename TEnum>
	FString GetEnumValueAsString(const FString& Name, TEnum Value);

	// 根据字符串获取Enum值
	template<typename TEnum>
	TEnum GetEnumValueFromString(const FString& Name, FString Value);

private:
	static TSharedPtr<SDDataHandle> DataInstance;	// 单例实例

};
