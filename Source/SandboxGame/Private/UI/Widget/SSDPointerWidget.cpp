// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDPointerWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Materials/MaterialInstanceDynamic.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDPointerWidget::Construct(const FArguments& InArgs)
{
	// 获取GameStyle的蓝图样式
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");
	
	// 初始化大小为20
	CurrentSize = 20.f;
	// 初始未瞄准
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
	
	// 加载材质实例
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> StaticPointerMaterialInstance(TEXT(
		"MaterialInstanceConstant'/Game/Material/PointerMatInst.PointerMatInst'"));

	// 转换为动态材质实例
	PointerMaterial = (UMaterialInstanceDynamic*)StaticPointerMaterialInstance.Object;
}

void SSDPointerWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// 实时修改控件大小
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
