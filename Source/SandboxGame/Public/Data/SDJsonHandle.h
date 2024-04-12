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

private:
	// 读取Json文件到字符串
	bool LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString);

	// 保存Json字符串到文件
	bool WriteFileWithJsonData(const FString& FileName, const FString& RelaPath, const FString& JsonStr);

	// 将FJsonObject转换为Json格式字符串
	bool GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr);

private:
	// 存档文件名
	FString RecordDataFileName;

	// 相对路径
	FString RelativePath;
};
