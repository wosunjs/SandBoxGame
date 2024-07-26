// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/SDTypes.h"

struct FSDGameStyle;

// �ϳ�����ί��
DECLARE_DELEGATE(FCompoundInput)
// �ϳ���ȡί�У���������Ʒ��ţ���Ʒ����
DECLARE_DELEGATE_TwoParams(FCompoundOutput, int, int)
// ������Ʒί�У���������Ʒ��ţ���Ʒ����
DECLARE_DELEGATE_TwoParams(FThrowObject, int, int)
// �������������״̬������Ϸ�仯ί�У������ֱ��ǿ�������кţ����µ���ƷID��������Ʒ����
DECLARE_DELEGATE_ThreeParams(FPackageShortChange, int, int, int)

/**
 * 
 */
class SANDBOXGAME_API SSDContainerBaseWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDContainerBaseWidget)
	{}
	// ʹ����Ĳ��������ڸ���
	SLATE_ATTRIBUTE(int, WorkIndex)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// ��ȡʵ��
	static TSharedPtr<SSDContainerBaseWidget> CreateContainer(EContainerType::Type NeedType, int WorkID);

	// ��������ƶ��������ϵ�״̬
	void UpdateHovered(bool IsHovered);

	// ������������
	virtual void ResetContainerPara(int ObjectID, int Num);

	// ��ȡ��������ƷID������
	int GetIndex() const;
	int GetNum() const;

	// ����������������ID��Num���Լ���������������ƷID��������
	virtual void LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum);

	// �Ҽ��������
	virtual void RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum);

	// �Ƿ�Ϊ��
	bool IsEmpty();

	// �Ƿ��пռ�
	bool RemainSpace(int ObjectID);

	// ���һ��Ԫ��
	void AddObject(int ObjectID);

public:
	// �ϳ�����ί��
	FCompoundInput CompoundInput;
	// �ϳ����ί��
	FCompoundOutput CompoundOutput;
	// ��Ʒ����ί��
	FThrowObject ThrowObject;
	// ���������ί��
	FPackageShortChange PackageShortChange;

protected:
	// ��ȡ��Ʒ�Ƿ���Ե���
	bool MultiplyAble(int ObjectID);

protected:
	// ��ȡGameWidgetStyle
	const FSDGameStyle* GameStyle;

	// �������,���ڱ궨�ڱ����е�λ��
	TAttribute<int> WorkIndex;

	// �������
	TSharedPtr<SBorder> ContainerBorder;
	TSharedPtr<SBorder> ObjectImage;
	TSharedPtr<STextBlock> ObjectNumText;

	// �Ƿ���Hover״̬
	bool IsHover;

	// ��Ʒ���
	int ObjectIndex;
	// ��Ʒ����
	int ObjectNum;
};
