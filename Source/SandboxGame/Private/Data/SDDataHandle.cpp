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

}

void SDDataHandle::InitializeMenuAudio()
{
	// ͨ��SDStyle��ȡSDMenuWidgetStyle�ڱ༭���е�MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// �����Դ�ļ�����Դ�б�
	TArray<USoundCue*> MusicList;
	MusicList.Add(Cast<USoundCue>(MenuStyle->MenuBackgroundMusic.GetResourceObject()));

	TArray<USoundCue*> SoundList;
	SoundList.Add(Cast<USoundCue>(MenuStyle->StartGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->ExitGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->MenuItemChangeSound.GetResourceObject()));

	// �����Դ��Map��
	MenuAudioResource.Add(FString("Music"), MusicList);
	MenuAudioResource.Add(FString("Sound"), SoundList);

	// ��������
	ChangeVolume(MusicVolume, SoundVolume);

}

SDDataHandle::SDDataHandle()
{
	// ��ʼ��Ϊ�浵����
	InitRecordData();

	// ��ʼ����������
	InitializeMenuAudio();

	// ��ʼ����Ϸ����
	InitGameData();
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

		// ѭ�����ñ�����������
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Music"))->CreateIterator()); It; ++It) {
			// ��������
			(*It)->VolumeMultiplier = MusicVolume;
		}
	}

	if (SoundValue > 0) {
		SoundVolume = SoundValue;

		// ѭ��������Ч����
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Sound"))->CreateIterator()); It; ++It) {
			(*It)->VolumeMultiplier = SoundVolume;
		}
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

void SDDataHandle::InitGameData()
{
	// ���������Ϸ���ݳ�ʼ��
	InitObjectAttr();		// ��ʼ����Ʒ����
	InitResourceAttrMap();	// ��ʼ����Դ����
	InitCompoundTableMap();	// ��ʼ���ϳɱ�
}

void SDDataHandle::InitObjectAttr()
{
	SDSingleton<SDJsonHandle>::Get()->ObjectAttrJsonRead(ObjectAttrMap);

	// ��ȡGameStyle
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	// ����ˢ����
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
