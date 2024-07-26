// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Package/SSDContainerInputWidget.h"
#include "SlateOptMacros.h"

#include "common/SDHelper.h"
#include "Blueprint/UserWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDContainerInputWidget::Construct(const FArguments& InArgs)
{
	SSDContainerBaseWidget::Construct(
		SSDContainerBaseWidget::FArguments()
		.WorkIndex(InArgs._WorkIndex)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDContainerInputWidget::ResetContainerPara(int ObjectID, int Num)
{
	// 检查是否改变
	bool IsChanged = (ObjectIndex != ObjectID || ObjectNum != Num);
	
	// 调用父类实现
	SSDContainerBaseWidget::ResetContainerPara(ObjectID, Num);

	// 如果有改变，则执行合成输入委托
	if (IsChanged) CompoundInput.ExecuteIfBound();
}
