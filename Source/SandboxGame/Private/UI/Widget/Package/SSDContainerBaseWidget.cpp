// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Package/SSDContainerBaseWidget.h"
#include "SlateOptMacros.h"

#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "UI/Widget/Package/SSDContainerInputWidget.h"
#include "UI/Widget/Package/SSDContainerNormalWidget.h"
#include "UI/Widget/Package/SSDContainerOutputWidget.h"
#include "UI/Widget/Package/SSDContainerShortcutWidget.h"
#include "Data/SDDataHandle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDContainerBaseWidget::Construct(const FArguments& InArgs)
{
	// ��ȡGameStyle����ͼ��ʽ
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	// ��ȡ������
	WorkIndex = InArgs._WorkIndex;

	ChildSlot
	[
		SAssignNew(ContainerBorder, SBorder)
			.BorderImage(&GameStyle->NormalContainerBrush)
			.Padding(FMargin(8.f))
			[
				SAssignNew(ObjectImage, SBorder)
					.BorderImage(&GameStyle->EmptyBrush)
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(0.f, 0.f, 4.f, 0.f)) 
					[
						SAssignNew(ObjectNumText, STextBlock)
							.Font(GameStyle->Font_Outline_16)
							.ColorAndOpacity(GameStyle->FontColor_Black)
					]
			]
	];

	// ��ʼ������ͣ
	IsHover = false;

	// ��ʼ����ƷID������
	ObjectIndex = 0;
	ObjectNum = 0;
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedPtr<SSDContainerBaseWidget> SSDContainerBaseWidget::CreateContainer(EContainerType::Type NeedType, int WorkID)
{
	TSharedPtr<SSDContainerBaseWidget> ResultContainer;
	switch (NeedType)
	{
	case EContainerType::Output:
		SAssignNew(ResultContainer, SSDContainerOutputWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Input:
		SAssignNew(ResultContainer, SSDContainerInputWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Normal:
		SAssignNew(ResultContainer, SSDContainerNormalWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Shortcut:
		SAssignNew(ResultContainer, SSDContainerShortcutWidget).WorkIndex(WorkID);
		break;
	}
	return ResultContainer;
}

void SSDContainerBaseWidget::UpdateHovered(bool IsHovered)
{
	// �����ǰ֡��֮ǰ״̬��һ����������ͣ��ʾ
	if (IsHovered && !IsHover) {
		ContainerBorder->SetBorderImage(&GameStyle->ChoosedContainerBrush);
	}
	else if(!IsHovered && IsHover){
		ContainerBorder->SetBorderImage(&GameStyle->NormalContainerBrush);
	}
	// ���µ�ǰ״̬
	IsHover = IsHovered;
}

void SSDContainerBaseWidget::ResetContainerPara(int ObjectID, int Num)
{
	// �������ID����ͬ���������ͼ
	if (ObjectIndex != ObjectID) {
		ObjectImage->SetBorderImage(SDDataHandle::Get()->ObjectBrushList[ObjectID]);
	}
	ObjectIndex = ObjectID;
	ObjectNum = Num;

	// �����ƷIDΪ0������Ϊ1������ʾ����
	if (ObjectID == 0 || Num <= 1) {
		ObjectNumText->SetText(FText::FromString(""));
	}
	else {
		// ��ʾ����
		ObjectNumText->SetText(FText::FromString(FString::FromInt(Num)));
	}
}

int SSDContainerBaseWidget::GetIndex() const
{
	return ObjectIndex;
}

int SSDContainerBaseWidget::GetNum() const
{
	return ObjectNum;
}

void SSDContainerBaseWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	// ���������Ʒ����ԭʼ��������Ʒ��ͬ�ҿ��Ե���
	if (InputID == ObjectIndex && MultiplyAble(ObjectIndex)) {
		// ���������жϷ��ص�ID
		OutputID = (ObjectNum + InputNum <= 64) ? 0 : InputID;
		// �������С��64�򷵻�0
		OutputNum = (ObjectNum + InputNum <= 64) ? 0 : (ObjectNum + InputNum - 64);
		// ���ñ�����������Ϊ64
		ObjectNum = (ObjectNum + InputNum) % 65;
		// ��������
		ResetContainerPara(ObjectIndex, ObjectNum);
		return;
	}
	// �����ͬ��Ի�
	else {
		OutputID = ObjectIndex;
		OutputNum = ObjectNum;
		// ��������
		ResetContainerPara(InputID, InputNum);
	}
}

void SSDContainerBaseWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	// �������Ϊ�գ���ֱ�ӽ���ǰ������һ��ȡ��
	if (InputID == 0) {
		OutputID = ObjectIndex;
		OutputNum = (ObjectNum + 1) / 2;
		// ��������
		ObjectNum -= OutputNum;
		ResetContainerPara(ObjectNum == 0 ? 0 : ObjectIndex, ObjectNum);
		return;
	}
	else {
		// �����Ʒ��ͬ�ҿ��Ժϲ����ߵ�ǰ����Ϊ�գ��������һ������ǰ����
		if (ObjectIndex == 0 || (InputID == ObjectIndex && MultiplyAble(InputID))) {
			// �޸�����
			if (ObjectNum + 1 <= 64) {
				OutputNum = InputNum - 1;
				ObjectNum += 1;
			}
			else {
				OutputNum = InputNum;
			}
			// ���������Ƿ�Ϊ0�������ID
			OutputID = (OutputNum == 0) ? 0 : InputID;
			// ��������
			ResetContainerPara(InputID, ObjectNum);
			return;
		}
		// �����������Ʒ���ҵ�ǰ�����ǿջ򲻿����ֳֵ��ӣ���ֱ�ӽ���
		else {
			OutputID = ObjectIndex;
			OutputNum = ObjectNum;
			// ��������
			ResetContainerPara(InputID, InputNum);
			return;
		}
	}
}

bool SSDContainerBaseWidget::IsEmpty()
{
	return ObjectIndex == 0;
}

// ��������Ϊ��ʱ������Ƿ�ɷ�����ƷObjectID
bool SSDContainerBaseWidget::RemainSpace(int ObjectID)
{
	if (ObjectIndex == ObjectID && ObjectNum < 64 && MultiplyAble(ObjectID)) {
		return true;
	}

	return false;
}

void SSDContainerBaseWidget::AddObject(int ObjectID) {
	// �����������Ϊ��
	if (ObjectIndex == 0) {
		ResetContainerPara(ObjectID, 1);
		return;
	}
	// ��������ǿյ���Ʒ��ͬ�ҿɵ���
	if (ObjectID == ObjectIndex && ObjectNum < 64 && MultiplyAble(ObjectID)) {
		ResetContainerPara(ObjectID, ObjectNum + 1);
	}
}

bool SSDContainerBaseWidget::MultiplyAble(int ObjectID)
{
	// ��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	// �����Ƿ��������򹤾�
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}
