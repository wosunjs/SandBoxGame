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

	// ������Ʒ����
	void ObjectAttrJsonRead(TMap<int, TSharedPtr<ObjectAttribute>>& ObjectAttrMap);

	// ������Դ��Ʒ����
	void ResourceAttrJsonRead(TMap<int, TSharedPtr<ResourceAttribute>>& ResourceAttrMap);

	// �����ϳɱ�
	void CompoundTableJsonRead(TArray<TSharedPtr<CompoundTable>>& CompoundTableMap);

private:
	// ��ȡJson�ļ����ַ���
	bool LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString);

	// ����Json�ַ������ļ�
	bool WriteFileWithJsonData(const FString& FileName, const FString& RelaPath, const FString& JsonStr);

	// ��FJsonObjectת��ΪJson��ʽ�ַ���
	bool GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr);

	// ��Stringת����ObjectType�ķ���
	EObjectType::Type StringToObjectType(const FString ArgStr);

	// ��Stringת����ResourceType�ķ���
	EResourceType::Type StringToResourceType(const FString ArgStr);

private:
	// �浵�ļ���
	FString RecordDataFileName;

	// ��Ʒ�����ļ���
	FString ObjectAttrFileName;

	// ��Դ��Ʒ�����ļ���
	FString ResourceAttrFileName;

	// �ϳɱ��ļ���
	FString CompoundTableFileName;

	// ���·��
	FString RelativePath;
};
