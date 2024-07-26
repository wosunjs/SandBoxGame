// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDPointerWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Materials/MaterialInstanceDynamic.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDPointerWidget::Construct(const FArguments& InArgs)
{
	// ��ȡGameStyle����ͼ��ʽ
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");
	
	// ��ʼ����СΪ20
	CurrentSize = 20.f;
	// ��ʼδ��׼
	IsAimed = false;

	ChildSlot
	[
		SAssignNew(RootBox, SBox)
			.WidthOverride(TAttribute<FOptionalSize>(this, &SSDPointerWidget::GetBoxWidget))
			.HeightOverride(TAttribute<FOptionalSize>(this, &SSDPointerWidget::GetBoxHeight))
			[
				SNew(SImage)
					.Image(&GameStyle->PointerBrush)
			]

	];
	
	// ���ز���ʵ��
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> StaticPointerMaterialInstance(TEXT(
		"MaterialInstanceConstant'/Game/Material/PointerMatInst.PointerMatInst'"));

	// ת��Ϊ��̬����ʵ��
	PointerMaterial = (UMaterialInstanceDynamic*)StaticPointerMaterialInstance.Object;
}

void SSDPointerWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// ʵʱ�޸Ŀؼ���С
	CurrentSize = IsAimed ? FMath::FInterpTo(CurrentSize, 130.f, InDeltaTime, 10.f) 
		: FMath::FInterpTo(CurrentSize, 20.f, InDeltaTime, 10.f);
}

void SSDPointerWidget::UpdatePointer(bool IsAim, float Range)
{
	IsAimed = IsAim;
	PointerMaterial->SetScalarParameterValue(FName("Range"), Range);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FOptionalSize SSDPointerWidget::GetBoxWidget() const
{
	return FOptionalSize();
}

FOptionalSize SSDPointerWidget::GetBoxHeight() const
{
	return FOptionalSize();
}
