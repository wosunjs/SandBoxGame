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
	// �Ƚ�Json�ļ��е����ݶ�ȡ��FString
	FString JsonValue;
	LoadStringFromFile(RecordDataFileName, RelativePath, JsonValue);

	// ͨ��Json��ȡ�����ഴ����ȡ������ʵ��
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	// ͨ��FJsonSerializer�ṩ�ķ����л��ӿڽ���JsonReader��JsonParsed
	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		// ��ȡ��������
		Culture = JsonParsed[0]->AsObject()->GetStringField(FString("Culture"));	// ��json�ĵ�0�����л�ȡCulture����Ӧ��ֵΪ�ַ�������
		MusicVolume = JsonParsed[1]->AsObject()->GetNumberField(FString("MusicVolume"));
		SoundVolume = JsonParsed[2]->AsObject()->GetNumberField(FString("SoundVolume"));

		// ��ȡ�浵����
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

	// �����������
	TSharedPtr<FJsonObject> CultureObject = MakeShareable(new FJsonObject);
	CultureObject->SetStringField("Culture", Culture);		// ��ȡ���ݣ�����ϼ�ֵ
	TSharedPtr<FJsonValueObject> CultureValue = MakeShareable(new FJsonValueObject(CultureObject));	//ת��Ϊ�ɲ����CultureValue

	// �����������
	TSharedPtr<FJsonObject> MusicVolumeObject = MakeShareable(new FJsonObject);
	MusicVolumeObject->SetNumberField("MusicVolume", MusicVolume);
	TSharedPtr<FJsonValueObject> MusicVolumeValue = MakeShareable(new FJsonValueObject(MusicVolumeObject));

	// �����Ч����
	TSharedPtr<FJsonObject> SoundVolumeObject = MakeShareable(new FJsonObject);
	SoundVolumeObject->SetNumberField("SoundVolume", SoundVolume);
	TSharedPtr<FJsonValueObject> SoundVolumeValue = MakeShareable(new FJsonValueObject(SoundVolumeObject));

	// ���浵����������
	TArray<TSharedPtr<FJsonValue>> RecordDataArray;
	for (int i = 0; i < RecordDataList->Num(); i++) {
		TSharedPtr<FJsonObject> RecordItem = MakeShareable(new FJsonObject);
		RecordItem->SetStringField(FString::FromInt(i), (*RecordDataList)[i]);
		TSharedPtr<FJsonValueObject> RecordDataValue = MakeShareable(new FJsonValueObject(RecordItem));
		RecordDataArray.Add(RecordDataValue);
	}
	TSharedPtr<FJsonObject> RecordDataObject = MakeShareable(new FJsonObject);
	RecordDataObject->SetArrayField("RecordData", RecordDataArray);	 // ��ӹؼ���key��ֵ
	TSharedPtr<FJsonValueObject> RecordDataValue = MakeShareable(new FJsonValueObject(RecordDataObject));	//����Json��ʽ����

	// ���ν����ݲ�������
	BaseDataArray.Add(CultureValue);
	BaseDataArray.Add(MusicVolumeValue);
	BaseDataArray.Add(SoundVolumeValue);
	BaseDataArray.Add(RecordDataValue);

	// ��ֵ��ϳ�JsonObject
	JsonObject->SetArrayField("T", BaseDataArray);

	// ��JsonObjectȥ��Keyת��Ϊ�ַ���
	FString JsonStr;
	GetFStringInJsonData(JsonObject, JsonStr);

	//SDHelper::Debug(FString("Origin str : " + JsonStr), 60.f);

	// ȥ�������ַ�
	JsonStr.RemoveAt(0, 8);
	JsonStr.RemoveFromEnd(FString("}"));

	//SDHelper::Debug(FString("Final str : " + JsonStr), 60.f);

	// д���ļ�
	WriteFileWithJsonData(RecordDataFileName, RelativePath, JsonStr);
}

bool SDJsonHandle::LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString)
{
	if (!FileName.IsEmpty()) {
		// ��ȡ����·��
		FString AbsoPath = FPaths::ProjectContentDir() + RelaPath + FileName;
		if (FPaths::FileExists(AbsoPath)) {
			if (FFileHelper::LoadFileToString(ResultString, *AbsoPath)) {
				return true;
			}
			else {
				// ���ز��ɹ�
				SDHelper::Debug(FString("Load Error") + AbsoPath);
				return false;
			}
		}
		else {
			// ���Debug
			SDHelper::Debug(FString("File Not Exist") + AbsoPath);
		}
	}

	return false;
}

bool SDJsonHandle::WriteFileWithJsonData(const FString& FileName, const FString& RelaPath, const FString& JsonStr)
{
	if (!JsonStr.IsEmpty() || !FileName.IsEmpty()) {
		FString AbsoPath = FPaths::ProjectContentDir() + RelaPath + FileName;
		// ��������
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
	// ��ָ�벻Ϊ����������
	if (JsonObj.IsValid() && JsonObj->Values.Num() > 0) {
		// ͨ��Jsonд�빤���ഴ��д�빤����ʵ��
		TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
		FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter);
		return true;
	}
	return false;
}

