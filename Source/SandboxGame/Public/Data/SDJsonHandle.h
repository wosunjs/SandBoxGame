// ����Json���ݵ���

#pragma once

#include "CoreMinimal.h"
#include "SDTypes.h"
#include "Json.h"

/**
 * 
 */
class SANDBOXGAME_API SDJsonHandle
{
public:
	SDJsonHandle();

	// �����浵
	void RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList);

	// �޸Ĵ浵
	void UpdateRecordData(FString Culture, float MusicVolume, float SoundVolume, TArray<FString>* RecordDataList);

private:
	// ��ȡJson�ļ����ַ���
	bool LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString);

	// ����Json�ַ������ļ�
	bool WriteFileWithJsonData(const FString& FileName, const FString& RelaPath, const FString& JsonStr);

	// ��FJsonObjectת��ΪJson��ʽ�ַ���
	bool GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr);

private:
	// �浵�ļ���
	FString RecordDataFileName;

	// ���·��
	FString RelativePath;
};
