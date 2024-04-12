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
	TSharedRef<SDDataHandle> DataRef = MakeShareable(new SDDataHandle());	// MakeShareable�����ڴ�������ָ�������
	return DataRef;
}

void SDDataHandle::InitRecordData()
{
	RecordName = FString("");

	// ��ȡ����
	FString Culture;
	
	// ��ȡ�浵
	SDSingleton<SDJsonHandle>::Get()->RecordDataJsonRead(Culture, MusicVolume, SoundVolume, RecordDataList);

	// ͨ���ַ�����ȡEeumֵ����ʼ������
	ChangeLocalizationCulture(GetEnumValueFromString<ECultureTeam>(FString("ECultureTeam"), Culture));


	//// �������
	//SDHelper::Debug(FString("Lan:") + Culture + FString("--") 
	//	+ FString("Mus:") + FString::SanitizeFloat(MusicVolume) + FString("--")
	//	+ FString("Sou:") + FString::SanitizeFloat(SoundVolume)
	//	,20.f
	//);

	//// ѭ����ȡRecordDataList
	//for (TArray<FString>::TIterator It(RecordDataList); It; ++It) {
	//	SDHelper::Debug(*It, 20.f);
	//}
}

SDDataHandle::SDDataHandle()
{
	// ��ʼ��Ϊ�浵����
	InitRecordData();

}

void SDDataHandle::ChangeLocalizationCulture(ECultureTeam Culture)
{
	bool SuccesChange = false;

	switch (Culture) {
	case ECultureTeam::EN:
		SuccesChange = FInternationalization::Get().SetCurrentCulture(TEXT("en"));		// �޸�ue4�Դ��Ĺ��ʻ�ʵ��
		break;
	case ECultureTeam::ZH:
		SuccesChange = FInternationalization::Get().SetCurrentCulture(TEXT("zh"));
		break;
	}

	if (!SuccesChange) {
		SDHelper::Debug(TEXT("Change Culture Fail!"));
	}

	CurrentCulture = Culture;

	// ���޸�д���ļ�
	SDSingleton<SDJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("EcultureTeam")
		, CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

ECultureTeam SDDataHandle::GetLocalizationCulture()
{
	return CurrentCulture;
}

void SDDataHandle::ChangeVolume(float MusicValue, float SoundValue)
{
	// ͨ�����븺�����ṩ�ӿڹ��ø������޸ı���
	if (MusicValue > 0) {
		MusicVolume = MusicValue;
	}

	if (SoundValue > 0) {
		SoundVolume = SoundValue;
	}

	// ���޸�д���ļ�
	SDSingleton<SDJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("EcultureTeam")
		, CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

template<typename TEnum>
inline FString SDDataHandle::GetEnumValueAsString(const FString& Name, TEnum Value)
{
	// ͨ��UE�����ANY_PACKAGE������Դ�ļ�������ΪName���࣬Ȼ��ʵ����һ��tmp����ȡ��ָ��
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