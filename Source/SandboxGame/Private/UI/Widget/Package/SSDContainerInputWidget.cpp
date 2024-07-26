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
	// ����Ƿ�ı�
	bool IsChanged = (ObjectIndex != ObjectID || ObjectNum != Num);
	
	// ���ø���ʵ��
	SSDContainerBaseWidget::ResetContainerPara(ObjectID, Num);

	// ����иı䣬��ִ�кϳ�����ί��
	if (IsChanged) CompoundInput.ExecuteIfBound();
}
