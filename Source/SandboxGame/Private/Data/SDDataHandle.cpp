// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SDDataHandle.h"
#include "Data/SDSingleton.h"
#include "Data/SDJsonHandle.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Common/SDHelper.h"
#include "Internationalization/Internationalization.h"
#include "Sound/SoundCue.h"

TSharedPtr<SDDataHandle> SDDataHandle::DataInstance = nullptr;

void SDDataHandle::Initialize()
{
	if (!DataInstance.IsValid()) {
		DataInstance = Create();
	}
}

TSharedPtr<SDDataHandle> SDDataHandle::Get()
{
	Initialize();
	return SDDataHandle::DataInstance;
}


TSharedPtr<SDDataHandle> SDDataHandle::Create()
{
	TSharedRef<SDDataHandle> DataRef = MakeShareable(new SDDataHandle());	// MakeShareable可用于创建共享指针和引用
	return DataRef;
}

void SDDataHandle::InitRecordData()
{
	RecordName = FString("");

	// 获取语言
	FString Culture;
	
	// 读取存档
	SDSingleton<SDJsonHandle>::Get()->RecordDataJsonRead(Culture, MusicVolume, SoundVolume, RecordDataList);

	// 通过字符串获取Eeum值并初始化语言
	ChangeLocalizationCulture(GetEnumValueFromString<ECultureTeam>(FString("ECultureTeam"), Culture));

}

void SDDataHandle::InitializeMenuAudio()
{
	// 通过SDStyle获取SDMenuWidgetStyle在编辑器中的MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// 添加资源文件到资源列表
	TArray<USoundCue*> MusicList;
	MusicList.Add(Cast<USoundCue>(MenuStyle->MenuBackgroundMusic.GetResourceObject()));

	TArray<USoundCue*> SoundList;
	SoundList.Add(Cast<USoundCue>(MenuStyle->StartGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->ExitGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->MenuItemChangeSound.GetResourceObject()));

	// 添加资源到Map中
	MenuAudioResource.Add(FString("Music"), MusicList);
	MenuAudioResource.Add(FString("Sound"), SoundList);

	// 重置声音
	ChangeVolume(MusicVolume, SoundVolume);

}

SDDataHandle::SDDataHandle()
{
	// 初始化为存档数据
	InitRecordData();

	// 初始化音乐数据
	InitializeMenuAudio();

	// 初始化游戏数据
	InitGameData();
}

void SDDataHandle::ChangeLocalizationCulture(ECultureTeam Culture)
{
	bool SuccesChange = false;

	switch (Culture) {
	case ECultureTeam::EN:
		SuccesChange = FInternationalization::Get().SetCurrentCulture(TEXT("en"));		// 修改ue4自带的国际化实例
		break;
	case ECultureTeam::ZH:
		SuccesChange = FInternationalization::Get().SetCurrentCulture(TEXT("zh"));
		break;
	}

	if (!SuccesChange) {
		SDHelper::Debug(TEXT("Change Culture Fail!"));
	}

	CurrentCulture = Culture;

	// 将修改写入文件
	SDSingleton<SDJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("EcultureTeam")
		, CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

ECultureTeam SDDataHandle::GetLocalizationCulture()
{
	return CurrentCulture;
}

void SDDataHandle::ChangeVolume(float MusicValue, float SoundValue)
{
	// 通过传入负数来提供接口共用给两个修改变量
	if (MusicValue > 0) {
		MusicVolume = MusicValue;

		// 循环设置背景音乐音量
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Music"))->CreateIterator()); It; ++It) {
			// 设置音量
			(*It)->VolumeMultiplier = MusicVolume;
		}
	}

	if (SoundValue > 0) {
		SoundVolume = SoundValue;

		// 循环设置音效音量
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Sound"))->CreateIterator()); It; ++It) {
			(*It)->VolumeMultiplier = SoundVolume;
		}
	}

	// 将修改写入文件
	SDSingleton<SDJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("EcultureTeam")
		, CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

template<typename TEnum>
inline FString SDDataHandle::GetEnumValueAsString(const FString& Name, TEnum Value)
{
	// 通过UE反射从ANY_PACKAGE所有资源文件中找名为Name的类，然后实例化一个tmp并获取其指针
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr) {
		return FString("Invalid");
	}
	
	return EnumPtr->GetEnumName((int32)Value);
}

template<typename TEnum>
inline TEnum SDDataHandle::GetEnumValueFromString(const FString& Name, FString Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr) {
		return TEnum(0);
	}

	return (TEnum)EnumPtr->GetIndexByName(FName(*FString(Value)));
}

void SDDataHandle::InitGameData()
{
	// 依次完成游戏数据初始化
	InitObjectAttr();		// 初始化物品属性
	InitResourceAttrMap();	// 初始化资源属性
	InitCompoundTableMap();	// 初始化合成表
}

void SDDataHandle::InitObjectAttr()
{
	SDSingleton<SDJsonHandle>::Get()->ObjectAttrJsonRead(ObjectAttrMap);

	// 获取GameStyle
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	// 填充笔刷数组
	ObjectBrushList.Add(&GameStyle->EmptyBrush);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_1);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_2);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_3);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_4);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_5);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_6);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_7);
	
}

void SDDataHandle::InitResourceAttrMap()
{
	SDSingleton<SDJsonHandle>::Get()->ResourceAttrJsonRead(ResourceAttrMap);
}

void SDDataHandle::InitCompoundTableMap()
{
	SDSingleton<SDJsonHandle>::Get()->CompoundTableJsonRead(CompoundTableMap);
}
