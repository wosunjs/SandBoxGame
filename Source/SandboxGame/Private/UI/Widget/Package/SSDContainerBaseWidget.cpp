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
	// 获取GameStyle的蓝图样式
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	// 获取工作号
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

	// 初始化无悬停
	IsHover = false;

	// 初始化物品ID和数量
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
	// 如果当前帧与之前状态不一样，跟新悬停显示
	if (IsHovered && !IsHover) {
		ContainerBorder->SetBorderImage(&GameStyle->ChoosedContainerBrush);
	}
	else if(!IsHovered && IsHover){
		ContainerBorder->SetBorderImage(&GameStyle->NormalContainerBrush);
	}
	// 跟新当前状态
	IsHover = IsHovered;
}

void SSDContainerBaseWidget::ResetContainerPara(int ObjectID, int Num)
{
	// 如果输入ID不相同，则更新贴图
	if (ObjectIndex != ObjectID) {
		ObjectImage->SetBorderImage(SDDataHandle::Get()->ObjectBrushList[ObjectID]);
	}
	ObjectIndex = ObjectID;
	ObjectNum = Num;

	// 如果物品ID为0或数量为1，则不显示数量
	if (ObjectID == 0 || Num <= 1) {
		ObjectNumText->SetText(FText::FromString(""));
	}
	else {
		// 显示数量
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
	// 如果输入物品与与原始容器中物品相同且可以叠加
	if (InputID == ObjectIndex && MultiplyAble(ObjectIndex)) {
		// 根据数量判断返回的ID
		OutputID = (ObjectNum + InputNum <= 64) ? 0 : InputID;
		// 如果数量小于64则返回0
		OutputNum = (ObjectNum + InputNum <= 64) ? 0 : (ObjectNum + InputNum - 64);
		// 设置本地数量上限为64
		ObjectNum = (ObjectNum + InputNum) % 65;
		// 更新属性
		ResetContainerPara(ObjectIndex, ObjectNum);
		return;
	}
	// 如果不同则对换
	else {
		OutputID = ObjectIndex;
		OutputNum = ObjectNum;
		// 更新属性
		ResetContainerPara(InputID, InputNum);
	}
}

void SSDContainerBaseWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	// 如果输入为空，则直接将当前容器的一半取出
	if (InputID == 0) {
		OutputID = ObjectIndex;
		OutputNum = (ObjectNum + 1) / 2;
		// 更新属性
		ObjectNum -= OutputNum;
		ResetContainerPara(ObjectNum == 0 ? 0 : ObjectIndex, ObjectNum);
		return;
	}
	else {
		// 如果物品相同且可以合并或者当前容器为空，则尝试添加一个到当前容器
		if (ObjectIndex == 0 || (InputID == ObjectIndex && MultiplyAble(InputID))) {
			// 修改数量
			if (ObjectNum + 1 <= 64) {
				OutputNum = InputNum - 1;
				ObjectNum += 1;
			}
			else {
				OutputNum = InputNum;
			}
			// 根据数量是否为0定义输出ID
			OutputID = (OutputNum == 0) ? 0 : InputID;
			// 更新属性
			ResetContainerPara(InputID, ObjectNum);
			return;
		}
		// 如果鼠标持有物品，且当前容器非空或不可与手持叠加，则直接交换
		else {
			OutputID = ObjectIndex;
			OutputNum = ObjectNum;
			// 更新属性
			ResetContainerPara(InputID, InputNum);
			return;
		}
	}
}

bool SSDContainerBaseWidget::IsEmpty()
{
	return ObjectIndex == 0;
}

// 当容器不为空时，检测是否可放入物品ObjectID
bool SSDContainerBaseWidget::RemainSpace(int ObjectID)
{
	if (ObjectIndex == ObjectID && ObjectNum < 64 && MultiplyAble(ObjectID)) {
		return true;
	}

	return false;
}

void SSDContainerBaseWidget::AddObject(int ObjectID) {
	// 如果容器本身为空
	if (ObjectIndex == 0) {
		ResetContainerPara(ObjectID, 1);
		return;
	}
	// 如果容器非空但物品相同且可叠加
	if (ObjectID == ObjectIndex && ObjectNum < 64 && MultiplyAble(ObjectID)) {
		ResetContainerPara(ObjectID, ObjectNum + 1);
	}
}

bool SSDContainerBaseWidget::MultiplyAble(int ObjectID)
{
	// 获取物品属性
	TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	// 返回是否是武器或工具
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}
