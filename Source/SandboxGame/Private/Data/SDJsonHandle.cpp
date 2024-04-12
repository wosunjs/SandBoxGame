// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SDJsonHandle.h"
#include "Common/SDHelper.h"

SDJsonHandle::SDJsonHandle()
{
	RecordDataFileName = FString("RecordData.json");
	RelativePath = FString("Res/ConfigData/");

}

void SDJsonHandle::RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList)
{
	// 先将Json文件中的数据读取到FString
	FString JsonValue;
	LoadStringFromFile(RecordDataFileName, RelativePath, JsonValue);

	// 通过Json读取工厂类创建读取工具类实例
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	// 通过FJsonSerializer提供的反序列化接口解析JsonReader到JsonParsed
	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		// 获取配置数据
		Culture = JsonParsed[0]->AsObject()->GetStringField(FString("Culture"));	// 从json的第0数组中获取Culture键对应的值为字符串类型
		MusicVolume = JsonParsed[1]->AsObject()->GetNumberField(FString("MusicVolume"));
		SoundVolume = JsonParsed[2]->AsObject()->GetNumberField(FString("SoundVolume"));

		// 获取存档数据
		TArray<TSharedPtr<FJsonValue>> RecordDataArray = JsonParsed[3]->AsObject()->GetArrayField(FString("RecordData"));
		for (int i = 0; i < RecordDataArray.Num(); ++i) {
			FString RecordDataName = RecordDataArray[i]->AsObject()->GetStringField(FString::FromInt(i));
			RecordDataList.Add(RecordDataName);
		}
	}
	else {
		SDHelper::Debug(FString("Deserialize Failed!"));
	}
}

void SDJsonHandle::UpdateRecordData(FString Culture, float MusicVolume, float SoundVolume, TArray<FString>* RecordDataList)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> BaseDataArray;

	// 填充语言数据
	TSharedPtr<FJsonObject> CultureObject = MakeShareable(new FJsonObject);
	CultureObject->SetStringField("Culture", Culture);		// 读取数据，并组合键值
	TSharedPtr<FJsonValueObject> CultureValue = MakeShareable(new FJsonValueObject(CultureObject));	//转换为可插入的CultureValue

	// 填充音乐数据
	TSharedPtr<FJsonObject> MusicVolumeObject = MakeShareable(new FJsonObject);
	MusicVolumeObject->SetNumberField("MusicVolume", MusicVolume);
	TSharedPtr<FJsonValueObject> MusicVolumeValue = MakeShareable(new FJsonValueObject(MusicVolumeObject));

	// 填充音效数据
	TSharedPtr<FJsonObject> SoundVolumeObject = MakeShareable(new FJsonObject);
	SoundVolumeObject->SetNumberField("SoundVolume", SoundVolume);
	TSharedPtr<FJsonValueObject> SoundVolumeValue = MakeShareable(new FJsonValueObject(SoundVolumeObject));

	// 填充存档名数组数据
	TArray<TSharedPtr<FJsonValue>> RecordDataArray;
	for (int i = 0; i < RecordDataList->Num(); i++) {
		TSharedPtr<FJsonObject> RecordItem = MakeShareable(new FJsonObject);
		RecordItem->SetStringField(FString::FromInt(i), (*RecordDataList)[i]);
		TSharedPtr<FJsonValueObject> RecordDataValue = MakeShareable(new FJsonValueObject(RecordItem));
		RecordDataArray.Add(RecordDataValue);
	}
	TSharedPtr<FJsonObject> RecordDataObject = MakeShareable(new FJsonObject);
	RecordDataObject->SetArrayField("RecordData", RecordDataArray);	 // 添加关键字key和值
	TSharedPtr<FJsonValueObject> RecordDataValue = MakeShareable(new FJsonValueObject(RecordDataObject));	//创建Json格式数据

	// 依次将数据插入数组
	BaseDataArray.Add(CultureValue);
	BaseDataArray.Add(MusicVolumeValue);
	BaseDataArray.Add(SoundVolumeValue);
	BaseDataArray.Add(RecordDataValue);

	// 键值组合成JsonObject
	JsonObject->SetArrayField("T", BaseDataArray);

	// 将JsonObject去掉Key转换为字符串
	FString JsonStr;
	GetFStringInJsonData(JsonObject, JsonStr);

	//SDHelper::Debug(FString("Origin str : " + JsonStr), 60.f);

	// 去掉多余字符
	JsonStr.RemoveAt(0, 8);
	JsonStr.RemoveFromEnd(FString("}"));

	//SDHelper::Debug(FString("Final str : " + JsonStr), 60.f);

	// 写入文件
	WriteFileWithJsonData(RecordDataFileName, RelativePath, JsonStr);
}

bool SDJsonHandle::LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString)
{
	if (!FileName.IsEmpty()) {
		// 获取绝对路径
		FString AbsoPath = FPaths::ProjectContentDir() + RelaPath + FileName;
		if (FPaths::FileExists(AbsoPath)) {
			if (FFileHelper::LoadFileToString(ResultString, *AbsoPath)) {
				return true;
			}
			else {
				// 加载不成功
				SDHelper::Debug(FString("Load Error") + AbsoPath);
				return false;
			}
		}
		else {
			// 输出Debug
			SDHelper::Debug(FString("File Not Exist") + AbsoPath);
		}
	}

	return false;
}

bool SDJsonHandle::WriteFileWithJsonData(const FString& FileName, const FString& RelaPath, const FString& JsonStr)
{
	if (!JsonStr.IsEmpty() || !FileName.IsEmpty()) {
		FString AbsoPath = FPaths::ProjectContentDir() + RelaPath + FileName;
		// 保存数据
		if (FFileHelper::SaveStringToFile(JsonStr, *AbsoPath)) {
			return true;
		}
		else {
			SDHelper::Debug(FString("Save File '") + AbsoPath + FString("' Failed!"), 10.f);
		}
	}

	return false;
}

bool SDJsonHandle::GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr)
{
	// 若指针不为空且有数据
	if (JsonObj.IsValid() && JsonObj->Values.Num() > 0) {
		// 通过Json写入工厂类创建写入工具类实例
		TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
		FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter);
		return true;
	}
	return false;
}

