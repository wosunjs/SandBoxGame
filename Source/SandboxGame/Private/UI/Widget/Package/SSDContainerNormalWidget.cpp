// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Package/SSDContainerNormalWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDContainerNormalWidget::Construct(const FArguments& InArgs)
{
	SSDContainerBaseWidget::Construct(
		SSDContainerBaseWidget::FArguments()
		.WorkIndex(InArgs._WorkIndex)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
