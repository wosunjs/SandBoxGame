// 处理Json数据的类

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

	// 解析存档
	void RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList);

	// 修改存档
	void UpdateRecordData(FString Culture, float MusicVolume, float SoundVolume, TArray<FString>* RecordDataList);

	// 解析物品属性
	void ObjectAttrJsonRead(TMap<int, TSharedPtr<ObjectAttribute>>& ObjectAttrMap);

	// 解析资源物品属性
	void ResourceAttrJsonRead(TMap<int, TSharedPtr<ResourceAttribute>>& ResourceAttrMap);

	// 解析合成表
	void CompoundTableJsonRead(TArray<TSharedPtr<CompoundTable>>& CompoundTableMap);

private:
	// 读取Json文件到字符串
	bool LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString);

	// 保存Json字符串到文件
	bool WriteFileWithJsonData(const FString& FileName, const FString& RelaPath, const FString& JsonStr);

	// 将FJsonObject转换为Json格式字符串
	bool GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr);

	// 从String转换到ObjectType的方法
	EObjectType::Type StringToObjectType(const FString ArgStr);

	// 从String转换到ResourceType的方法
	EResourceType::Type StringToResourceType(const FString ArgStr);

private:
	// 存档文件名
	FString RecordDataFileName;

	// 物品属性文件名
	FString ObjectAttrFileName;

	// 资源物品属性文件名
	FString ResourceAttrFileName;

	// 合成表文件名
	FString CompoundTableFileName;

	// 相对路径
	FString RelativePath;
};
