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
	// ����Ƿ�ı�
	bool IsChanged = (ObjectIndex != ObjectID || ObjectNum != Num);

	// ���ø���ʵ��
	SSDContainerBaseWidget::ResetContainerPara(ObjectID, Num);

	// ����иı䣬ͨ�����ÿ��������ί�и��¿����
	if (IsChanged) PackageShortChange.ExecuteIfBound(WorkIndex.Get(), ObjectID, Num);

}