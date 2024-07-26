// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SDJsonHandle.h"
#include "Common/SDHelper.h"

SDJsonHandle::SDJsonHandle()
{
	// ��ʼ���ļ�·��
	RecordDataFileName = FString("RecordData.json");
	ObjectAttrFileName = FString("ObjectAttribute.json");
	ResourceAttrFileName = FString("ResourceAttribute.json");
	CompoundTableFileName = FString("CompoundTable.json");

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

void SDJsonHandle::ObjectAttrJsonRead(TMap<int, TSharedPtr<ObjectAttribute>>& ObjectAttrMap)
{
	// �Ƚ�Json�ļ��е����ݶ�ȡ��FString
	FString JsonValue;
	LoadStringFromFile(ObjectAttrFileName, RelativePath, JsonValue);

	// ͨ��Json��ȡ�����ഴ����ȡ������ʵ��
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	// ͨ��FJsonSerializer�ṩ�ķ����л��ӿڽ���JsonReader��JsonParsed
	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		// ��ȡ��������
		for (int i = 0; i < JsonParsed.Num(); ++i) {
			TArray<TSharedPtr<FJsonValue>> ObjectAttr = JsonParsed[i]->AsObject()->GetArrayField(FString::FromInt(i));
			FText EN = FText::FromString(ObjectAttr[0]->AsObject()->GetStringField("EN"));
			FText ZH = FText::FromString(ObjectAttr[1]->AsObject()->GetStringField("ZH"));
			FString ObjectTypeStr = ObjectAttr[2]->AsObject()->GetStringField("ObjectType");
			int PlantAttack = ObjectAttr[3]->AsObject()->GetIntegerField("PlantAttack");
			int MetalAttack = ObjectAttr[4]->AsObject()->GetIntegerField("MetalAttcck");
			int AnimalAttack = ObjectAttr[5]->AsObject()->GetIntegerField("AnimalAttack");
			int AffectRange = ObjectAttr[6]->AsObject()->GetIntegerField("AffectRange");
			FString TexPath = ObjectAttr[7]->AsObject()->GetStringField("TexPath");

			EObjectType::Type ObjectType = StringToObjectType(ObjectTypeStr);
			TSharedPtr<ObjectAttribute> ObjectAttrPtr = MakeShareable(new ObjectAttribute(EN, ZH, ObjectType
				, PlantAttack, MetalAttack, AnimalAttack, AffectRange, TexPath));

			ObjectAttrMap.Add(i, ObjectAttrPtr);
		}
	}
	else {
		SDHelper::Debug(FString("Deserialize Failed!"));
	}
}


void SDJsonHandle::ResourceAttrJsonRead(TMap<int, TSharedPtr<ResourceAttribute>>& ResourceAttrMap)
{
	FString JsonValue;
	LoadStringFromFile(ResourceAttrFileName, RelativePath, JsonValue);

	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		for (int i = 0; i < JsonParsed.Num(); ++i) {
			// ������Դ����
			TArray<TSharedPtr<FJsonValue>> ResourceAttr = JsonParsed[i]->AsObject()->GetArrayField(FString::FromInt(i+1));
			// ����Ӣ����
			FText EN = FText::FromString(ResourceAttr[0]->AsObject()->GetStringField("EN"));
			// ����������
			FText ZH = FText::FromString(ResourceAttr[1]->AsObject()->GetStringField("ZH"));
			
			// ��������
			EResourceType::Type ResourceType = StringToResourceType(ResourceAttr[2]->AsObject()->GetStringField("ResourceType"));
			// ����HPֵ
			int HP = ResourceAttr[3]->AsObject()->GetIntegerField("HP");

			// ���������Ｏ��
			TArray<TArray<int>> FlobObjectInfoArray;
			// ����������������Ϣ
			TArray<TSharedPtr<FJsonValue>> FlobObjectInfo = ResourceAttr[4]->AsObject()->GetArrayField(FString("FlobObjectInfo"));
			for (int j = 0; j < FlobObjectInfo.Num(); ++j) {
				FString FlobObjectInfoItem = FlobObjectInfo[j]->AsObject()->GetStringField(FString::FromInt(j));
				FString ObjectIndexStr;
				FString RangeStr;
				FString RangeMinStr;
				FString RangeMaxStr;
				FlobObjectInfoItem.Split(FString("_"), &ObjectIndexStr, &RangeStr);
				RangeStr.Split(FString(","), &RangeMinStr, &RangeMaxStr);

				TArray<int> FlobObjectInfoList;
				FlobObjectInfoList.Add(FCString::Atoi(*ObjectIndexStr));
				FlobObjectInfoList.Add(FCString::Atoi(*RangeMinStr));
				FlobObjectInfoList.Add(FCString::Atoi(*RangeMaxStr));
				// ��������������Ϣ��ӽ�����
				FlobObjectInfoArray.Add(FlobObjectInfoList);
			}

			TSharedPtr<ResourceAttribute> ResourceAttrPtr = MakeShareable(new ResourceAttribute(EN, ZH, ResourceType, HP, &FlobObjectInfoArray));
			ResourceAttrMap.Add(i + 1, ResourceAttrPtr);
		}
	}
	else {
		SDHelper::Debug(FString("Deserialize Failed!"));
	}
}

void SDJsonHandle::CompoundTableJsonRead(TArray<TSharedPtr<CompoundTable>>& CompoundTableMap)
{
	FString JsonValue;
	LoadStringFromFile(CompoundTableFileName, RelativePath, JsonValue);

	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		for (int i = 0; i < JsonParsed.Num(); ++i) {
			TArray<TSharedPtr<FJsonValue>> ObjectAttr = JsonParsed[i]->AsObject()->GetArrayField(FString::FromInt(i));

			TArray<int> CompoundTableAttr;
			for (int j = 0; j < 10; ++j) {
				CompoundTableAttr.Add(ObjectAttr[j]->AsObject()->GetIntegerField(FString::FromInt(j)));
			}

			TSharedPtr<CompoundTable> NewTable = MakeShareable(new CompoundTable(&CompoundTableAttr));

			CompoundTableMap.Add(NewTable);
		}
	}
	else {
		SDHelper::Debug(FString("Compound Table Deserialize Failed!"));
	}
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

EObjectType::Type SDJsonHandle::StringToObjectType(const FString ArgStr)
{
	if (ArgStr.Equals(FString("Normal"))) return EObjectType::Normal;
	else if (ArgStr.Equals(FString("Food"))) return EObjectType::Food;
	else if (ArgStr.Equals(FString("Tool"))) return EObjectType::Tool;
	else if (ArgStr.Equals(FString("Weapon"))) return EObjectType::Weapon;
	
	SDHelper::Debug(FString("EObjectType undefine!"));
	return EObjectType::Type();
}

EResourceType::Type SDJsonHandle::StringToResourceType(const FString ArgStr)
{
	if (ArgStr.Equals(FString("Plant"))) return EResourceType::Plant;
	else if (ArgStr.Equals(FString("Metal"))) return EResourceType::Metal;
	else if (ArgStr.Equals(FString("Animal"))) return EResourceType::Animal;
	return EResourceType::Type();
}

