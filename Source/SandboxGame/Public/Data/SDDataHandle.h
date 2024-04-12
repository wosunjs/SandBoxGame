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

	// �޸���Ӣ��
	void ChangeLocalizationCulture(ECultureTeam Culture);

	ECultureTeam GetLocalizationCulture();

	// �޸����� 
	void ChangeVolume(float MusicValue, float SoundValue);

public:
	// ����
	float MusicVolume;

	// ��Ч
	float SoundVolume;

	// �浵����
	TArray<FString> RecordDataList;

	// ��ǰ�浵��
	FString RecordName;

protected:
	// ���ػ�����
	ECultureTeam CurrentCulture;

private:
	static TSharedPtr<SDDataHandle> Create();	// ��������

	// ��ȡ�浵���ݽ��г�ʼ��
	void InitRecordData();

	// ����enum���ͻ�ȡ�ַ���(����UEnum����ͨ��ֵ�����ַ���)
	template<typename TEnum>
	FString GetEnumValueAsString(const FString& Name, TEnum Value);

	// �����ַ�����ȡEnumֵ
	template<typename TEnum>
	TEnum GetEnumValueFromString(const FString& Name, FString Value);

private:
	static TSharedPtr<SDDataHandle> DataInstance;	// ����ʵ��

};
