// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDPackageManager.h"
#include "Data/SDDataHandle.h"
#include "common/SDHelper.h"

// ����ָ���ʼ����ֵ
TSharedPtr<SDPackageManager> SDPackageManager::PackageInstance = nullptr;

SDPackageManager::SDPackageManager()
{
	// ��ʼ����ƷID������Ϊ0
	ObjectIndex = 0;
	ObjectNum = 0;
}

void SDPackageManager::Initialize()
{
	if (PackageInstance == nullptr) {
		PackageInstance = Create();
	}
}

TSharedPtr<SDPackageManager> SDPackageManager::Get()
{
	Initialize();
	return PackageInstance;
}

void SDPackageManager::InsertContainer(TSharedPtr<SSDContainerBaseWidget> Container, EContainerType::Type InsertType)
{
	switch (InsertType)
	{
	case EContainerType::Output:
		OutputContainer = Container;
		// �󶨺ϳ����ί�кͶ���ί��
		OutputContainer->CompoundOutput.BindRaw(this, &SDPackageManager::CompoundOutput);
		OutputContainer->ThrowObject.BindRaw(this, &SDPackageManager::ThrowObject);
		break;
	case EContainerType::Input:
		// �󶨺ϳ�����ί��
		Container->CompoundInput.BindRaw(this, &SDPackageManager::CompoundInput);
		InputContainerList.Add(Container);
		break;
	case EContainerType::Normal:
		NormalContainerList.Add(Container);
		break;
	case EContainerType::Shortcut:
		// �󶨿�����޸�ί��
		Container->PackageShortChange.BindRaw(this, &SDPackageManager::PackageShortChange);
		ShortcutContainerList.Add(Container);
		break;
	default:
		break;
	}
}

TSharedRef<SDPackageManager> SDPackageManager::Create()
{
	TSharedRef<SDPackageManager> PackageRef = MakeShareable(new SDPackageManager());
	return PackageRef;
}

void SDPackageManager::UpdateHovered(FVector2D MousePos, FGeometry PackGeo)
{
	// ��ȡ��ͣ����
	TSharedPtr<SSDContainerBaseWidget> CurrHoveredCon = LocateContainer(MousePos, PackGeo);

	// �����������
	if (CurrHoveredCon.IsValid()) {
		// ���õ�ǰ������ͣ��ʾ
		CurrHoveredCon->UpdateHovered(true);
		// �����һ�����������뵱ǰ��ͬ�����
		if (LastHoveredCon.IsValid() && LastHoveredCon.Get() != CurrHoveredCon.Get()) {
			// ������ͣ��ʾ
			LastHoveredCon->UpdateHovered(false);
		}
	}
	else {
		if (LastHoveredCon.IsValid()) {
			LastHoveredCon->UpdateHovered(false);
		}
	}

	// ������һ��ͣ����
	LastHoveredCon = CurrHoveredCon;
}

void SDPackageManager::LeftOption(FVector2D MousePos, FGeometry PackGeo)
{
	// �Ȼ�ȡ���������
	TSharedPtr<SSDContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	// ����������ڣ�ִ�������¼�
	if (ClickedContainer.IsValid()) {
		ClickedContainer->LeftOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
	// ������������ڣ�������ֳ���Ʒ��������Ʒ
	if (!ClickedContainer.IsValid() && ObjectIndex != 0) {
		// ������Ʒ
		ThrowObject(ObjectIndex, ObjectNum);
		// ��������
		ObjectIndex = ObjectNum = 0;
	}
}

void SDPackageManager::RightOption(FVector2D MousePos, FGeometry PackGeo)
{
	// �Ȼ�ȡ���������
	TSharedPtr<SSDContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	// ����������ڣ���ִ���¼�
	if (ClickedContainer.IsValid()) {
		ClickedContainer->RightOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
}

TSharedPtr<SSDContainerBaseWidget> SDPackageManager::LocateContainer(FVector2D MousePos, FGeometry PackGeo)
{
	// ������ڷֱ��ʷ�1920x1080���������ڿ���̨��������("-res 1920x1080")
	// �����ҵ�ָ����������������������ϳɴ��ڣ�
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It) {
		// ��ȡ������������(���Ϻ�����)
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);	 // ʹ�õ��Ǿ��Դ�С
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
			return *It;
		}
	}

	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It) {
		// ��ȡ������������(���Ϻ�����)
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);	 // ʹ�õ��Ǿ��Դ�С
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
			return *It;
		}
	}

	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		// ��ȡ������������(���Ϻ�����)
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);	 // ʹ�õ��Ǿ��Դ�С
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
			return *It;
		}
	}

	// ��ȡ������������(���Ϻ�����)
	FVector2D StartPos = PackGeo.AbsoluteToLocal((OutputContainer)->GetCachedGeometry().AbsolutePosition);
	FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);	 // ʹ�õ��Ǿ��Դ�С
	if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
		return OutputContainer;
	}

	return nullptr;
}

void SDPackageManager::ThrowObject(int ObjectID, int Num)
{
	PlayerThrowObject.ExecuteIfBound(ObjectID, Num);
}

void SDPackageManager::PackageShortChange(int ShortcutID, int ObjectID, int Num)
{
	// ִ���ֳ���Ʒ�޸�ί��
	ChangeHandObject.ExecuteIfBound(ShortcutID, ObjectID, Num);
}

void SDPackageManager::CompoundOutput(int ObjectID, int Num)
{
	// �����ȡ��������Ϊ�գ�ֱ��return
	if (ObjectID == 0) {
		return;
	}

	// �ϳɱ�ṹ����
	TArray<int> TableMap;
	// ��ȡ�ϳɱ�
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		TableMap.Add((*It)->GetIndex());
	}
	TableMap.Add(ObjectID);

	// ������������
	TArray<int> ExpendMap;
	// �����ϳɱ�ͼ���ҵ���Ӧ�ϳɱ���п۳�
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SDDataHandle::Get()->CompoundTableMap); It; ++It) {
		// ����ҵ����ϵ�ֱ������ѭ��
		if ((*It)->DetectExpend(&TableMap, Num, ExpendMap)) {
			break;
		}
	}

	// �۳��ϳ�̨��Ʒ����
	for (int i = 0; i < 9; i++) {
		if (ExpendMap[i] != 0) {
			int InputNum = InputContainerList[i]->GetNum() - ExpendMap[i];
			int InputID = InputNum == 0 ? 0 : InputContainerList[i]->GetIndex();
			InputContainerList[i]->ResetContainerPara(InputID, InputNum);
		}
	}

}

void SDPackageManager::CompoundInput()
{
	// ��ȡ�ϳ�̨9����������ƷID������
	TArray<int> IDMap;
	TArray<int> NumMap;
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		IDMap.Add((*It)->GetIndex());
		NumMap.Add((*It)->GetNum());
	}

	// �ϳ�������
	int OutputIndex = 0;
	int OutputNum = 0;
	// ��ϳɱ����Ƿ�ɺϳ�
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SDDataHandle::Get()->CompoundTableMap); It; ++It) {
		(*It)->DetectTable(&IDMap, &NumMap, OutputIndex, OutputNum);
		// �������һ���ɺϳɣ���ֱ�ӽ���
		if (OutputIndex != 0) {
			break;
		}
	}

	// ��OutputContainer�����޸�
	if (MultiplyAble(OutputIndex)) {
		// ����ɵ�����ֱ���޸�
		OutputContainer->ResetContainerPara(OutputIndex, OutputNum);
	}
	else {
		// ���ɵ�����ֻ���һ��
		OutputContainer->ResetContainerPara(OutputIndex, 1);
	}
}

bool SDPackageManager::MultiplyAble(int ObjectID)
{
	// ��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	// �����Ƿ��������򹤾�
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}

bool SDPackageManager::SearchFreeSpace(int ObjectID)
{
	TSharedPtr<SSDContainerBaseWidget> FreeContainer;
	return SearchFreeSpace(ObjectID, FreeContainer);
}

bool SDPackageManager::SearchFreeSpace(int ObjectID, TSharedPtr<SSDContainerBaseWidget>& FreeContainer)
{
	// ����������
	TSharedPtr<SSDContainerBaseWidget> EmptyContainer;
	// ������������Ƿ��и���Ʒ�ĵ���
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It) {
		// ����������������Ҹ�����Ϊ����ָ�������
		if (!EmptyContainer.IsValid() && (*It)->IsEmpty()) {
			EmptyContainer = *It;
		}
		// ���������Ϊ�գ�����������ƷID��ͬ�ҿɵ��ӣ���ֱ�ӷ���
		if (!FreeContainer.IsValid() && (*It)->RemainSpace(ObjectID)) {
			FreeContainer = *It;
			return true;
		}
	}

	// �����������Ƿ��и���Ʒ�ĵ���
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It) {
		// ����������������Ҹ�����Ϊ����ָ�������
		if (!EmptyContainer.IsValid() && (*It)->IsEmpty()) {
			EmptyContainer = *It;
		}
		// ���������Ϊ�գ�����������ƷID��ͬ�ҿɵ��ӣ���ֱ�ӷ���
		if (!FreeContainer.IsValid() && (*It)->RemainSpace(ObjectID)) {
			FreeContainer = *It;
			return true;
		}
	}

	// ���û�е������Է���һ��������
	if (EmptyContainer.IsValid()) {
		FreeContainer = EmptyContainer;
		return true;
	}

	return false;
}

void SDPackageManager::AddObject(int ObjectID)
{
	TSharedPtr<SSDContainerBaseWidget> FreeContainer;
	if (SearchFreeSpace(ObjectID, FreeContainer)) {
		// �����Ʒ������
		FreeContainer->AddObject(ObjectID);
	}
}

bool SDPackageManager::EatUpEvent(int shortcutID)
{
	// ��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.
		Find(ShortcutContainerList[shortcutID]->GetIndex());
	// �����ʳ�������
	if (ObjectAttr->ObjectType == EObjectType::Food) {
		int NewNum = ShortcutContainerList[shortcutID]->GetNum() - 1;
		int NewIndex = NewNum == 0 ? 0 : ShortcutContainerList[shortcutID]->GetIndex();
		ShortcutContainerList[shortcutID]->ResetContainerPara(NewIndex, NewNum);
		return true;
	}

	return false;
}
