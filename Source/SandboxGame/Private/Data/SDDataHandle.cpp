// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SDDataHandle.h"
#include "Internationalization/Internationalization.h"
#include "Data/SDSingleton.h"
#include "Data/SDJsonHandle.h"
#include "common/SDHelper.h"

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


	//// 尝试输出
	//SDHelper::Debug(FString("Lan:") + Culture + FString("--") 
	//	+ FString("Mus:") + FString::SanitizeFloat(MusicVolume) + FString("--")
	//	+ FString("Sou:") + FString::SanitizeFloat(SoundVolume)
	//	,20.f
	//);

	//// 循环读取RecordDataList
	//for (TArray<FString>::TIterator It(RecordDataList); It; ++It) {
	//	SDHelper::Debug(*It, 20.f);
	//}
}

SDDataHandle::SDDataHandle()
{
	// 初始化为存档数据
	InitRecordData();

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
	}

	if (SoundValue > 0) {
		SoundVolume = SoundValue;
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