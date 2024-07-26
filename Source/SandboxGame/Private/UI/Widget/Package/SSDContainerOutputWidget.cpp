// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Package/SSDContainerOutputWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDContainerOutputWidget::Construct(const FArguments& InArgs)
{
	SSDContainerBaseWidget::Construct(
		SSDContainerBaseWidget::FArguments()
		.WorkIndex(InArgs._WorkIndex)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDContainerOutputWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	// 如果本地物品为空，则无法将物体放入其中（输入啥就返回啥）
	if (ObjectIndex == 0) {
		OutputID = InputID;
		OutputNum = InputNum;
		return;
	}

	// 如果输入本地非空，且与输入物品相同且可叠加则将其全部叠加到输出
	if (InputID == ObjectIndex && MultiplyAble(InputID)) {
		OutputID = ObjectIndex;
		if (InputNum + ObjectNum <= 64) {
			OutputNum = InputNum + ObjectNum;
			ObjectNum = 0;
		}
		else {
			OutputNum = 64;
			ObjectNum = (InputNum + ObjectNum) - 64;
		}
		// 执行合成输出委托(减少合成材料)
		CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum - InputNum);
		return;
	}

	// 如果物品不相同或者相同不能叠加，输出物品，丢弃输入
	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
	// 合成输出委托
	CompoundOutput.ExecuteIfBound(ObjectIndex, ObjectNum);
	// 执行丢弃物品委托
	ThrowObject.ExecuteIfBound(InputID, InputNum);
}

void SSDContainerOutputWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	// 如果本地物品为空，则无操作
	if (ObjectIndex == 0) {
		OutputID = InputID;
		OutputNum = InputNum;
		return;
	}

	// 如果输入为空，则直接输出本地的一半
	if (InputID == 0) {
		OutputID = ObjectIndex;
		OutputNum = (ObjectNum + 1) / 2;
		// 执行合成输出委托
		CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum);
		// 更新属性

		return;
	}

	// 如果本地物品和输入物品相同且可合并，则给出一半
	if (InputID == ObjectIndex && MultiplyAble(InputID)) {
		OutputID = ObjectIndex;
		int num = FMath::Min((ObjectNum + 1) / 2, 64 - InputNum);
		OutputNum += num;
		ObjectNum -= num;
		// 执行合成输出委托
		CompoundOutput.ExecuteIfBound(ObjectIndex, num);
		return;
	}

	// 如果本地物品和输入物品不相同,则丢弃输入物品，输出本地物品的一半
	OutputID = ObjectIndex;
	OutputNum = (ObjectNum + 1) / 2;
	// 执行合成委托
	CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum);
	// 执行丢弃委托
	ThrowObject.ExecuteIfBound(InputID, InputNum);

}


