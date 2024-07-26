// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Package/SSDContainerShortcutWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDContainerShortcutWidget::Construct(const FArguments& InArgs)
{
	SSDContainerBaseWidget::Construct(
		SSDContainerBaseWidget::FArguments()
		.WorkIndex(InArgs._WorkIndex)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDContainerShortcutWidget::ResetContainerPara(int ObjectID, int Num)
{
	// 检查是否改变
	bool IsChanged = (ObjectIndex != ObjectID || ObjectNum != Num);

	// 调用父类实现
	SSDContainerBaseWidget::ResetContainerPara(ObjectID, Num);

	// 如果有改变，通过调用快捷栏更新委托更新快捷栏
	if (IsChanged) PackageShortChange.ExecuteIfBound(WorkIndex.Get(), ObjectID, Num);

}