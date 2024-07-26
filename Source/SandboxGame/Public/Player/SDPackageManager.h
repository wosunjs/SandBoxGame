// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SDTypes.h"
#include "UI/Widget/Package/SSDContainerBaseWidget.h"

/**
 * 
 */
class SANDBOXGAME_API SDPackageManager
{
public:
	SDPackageManager();
	
	static void Initialize();

	static TSharedPtr<SDPackageManager> Get();

	// �������
	void InsertContainer(TSharedPtr<SSDContainerBaseWidget> Container, EContainerType::Type InsertType);

	// ������ͣ��������ɫ
	void UpdateHovered(FVector2D MousePos, FGeometry PackGeo);

	// ����¼������������λ�ú�PackageWidget��Geometry
	void LeftOption(FVector2D MousePos, FGeometry PackGeo);

	// �Ҽ��¼������������λ�ú�PackageWidget��Geometry
	void RightOption(FVector2D MousePos, FGeometry PackGeo);

	// �����Ƿ��װ�����Ʒ
	bool SearchFreeSpace(int ObjectID);

	// �����Ʒ
	void AddObject(int ObjectID);

	// �Զ�������������ID�������Ƿ�ɹ��Ե�
	bool EatUpEvent(int shortcutID);

public:
	// ��������ƷID
	int ObjectIndex;

	// ��������Ʒ����
	int ObjectNum;

	// ������Ʒί�б������󶨵ķ�����PlayerState��PlayerThrowObject
	FThrowObject PlayerThrowObject;

	// �޸Ŀ����ί�б���
	FPackageShortChange ChangeHandObject;

private:
	// ����ʵ��
	static TSharedRef<SDPackageManager> Create();

	// ��ȡ���ָ�������
	TSharedPtr<SSDContainerBaseWidget> LocateContainer(FVector2D MousePos, FGeometry PackGeo);

	// ������Ʒ�¼�
	void ThrowObject(int ObjectID, int Num);

	// �ϳ���ȡ�¼�
	void CompoundOutput(int ObjectID, int Num);

	// �ϳ������¼�
	void CompoundInput();

	// ���������¼�
	void PackageShortChange(int ShortcutID, int ObjectID, int Num);

	// ��ȡ��Ʒ�Ƿ���Ե���
	bool MultiplyAble(int ObjectID);

	// ��ȡ�ɷ�����Ʒ�Ŀռ�
	bool SearchFreeSpace(int ObjectID, TSharedPtr<SSDContainerBaseWidget>& FreeContainer);

private:
	// ����ָ��
	static TSharedPtr<SDPackageManager> PackageInstance;

	// �����б�
	TArray<TSharedPtr<SSDContainerBaseWidget>> InputContainerList;
	TArray<TSharedPtr<SSDContainerBaseWidget>> NormalContainerList;
	TArray<TSharedPtr<SSDContainerBaseWidget>> ShortcutContainerList;
	// �������
	TSharedPtr<SSDContainerBaseWidget> OutputContainer;

	// ��һ��ͣ����
	TSharedPtr<SSDContainerBaseWidget> LastHoveredCon;
};
