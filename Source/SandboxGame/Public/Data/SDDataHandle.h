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

	// �޸���Ӣ��
	void ChangeLocalizationCulture(ECultureTeam Culture);

	// ��ȡ���ػ�
	ECultureTeam GetLocalizationCulture();

	// �޸����� 
	void ChangeVolume(float MusicValue, float SoundValue);

	// ��Ϸ���ݳ�ʼ��
	void InitGameData();

public:
	// ����
	float MusicVolume;

	// ��Ч
	float SoundVolume;

	// �浵����
	TArray<FString> RecordDataList;

	// ��ǰ�浵��
	FString RecordName;

	// ��Ʒ���Լ���
	TMap<int, TSharedPtr<ObjectAttribute>> ObjectAttrMap;

	// ��Ʒ��ͼ��Դ����
	TArray<const FSlateBrush*> ObjectBrushList;

	// ��Դ���Լ���
	TMap<int, TSharedPtr<ResourceAttribute>> ResourceAttrMap;

	// �ϳɱ�ͼ
	TArray<TSharedPtr<CompoundTable>> CompoundTableMap;

protected:
	// ���ػ�����
	ECultureTeam CurrentCulture;

private:
	// ��������
	static TSharedPtr<SDDataHandle> Create();

	// ��ȡ�浵���ݽ��г�ʼ��
	void InitRecordData();

	// ����enum���ͻ�ȡ�ַ���(����UEnum����ͨ��ֵ�����ַ���)
	template<typename TEnum>
	FString GetEnumValueAsString(const FString& Name, TEnum Value);

	// �����ַ�����ȡEnumֵ
	template<typename TEnum>
	TEnum GetEnumValueFromString(const FString& Name, FString Value);

	// ��ʼ��Menu��������
	void InitializeMenuAudio();

	// ��ʼ����Ʒ���Լ���
	void InitObjectAttr();

	// ��ʼ����Դ����ͼ
	void InitResourceAttrMap();

	// ��ʼ���ϳɱ�ͼ
	void InitCompoundTableMap();

private:
	static TSharedPtr<SDDataHandle> DataInstance;	// ����ʵ��

	// ��ȡMenu��ʽ
	const FSDMenuStyle* MenuStyle;

	// ��ȡGame��ʽ
	const FSDGameStyle* GameStyle;

	// ����Menu��������Դ
	TMap<FString, TArray<USoundCue*>> MenuAudioResource;

	//

};
