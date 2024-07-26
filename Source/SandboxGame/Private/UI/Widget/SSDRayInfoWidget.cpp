// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDRayInfoWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDRayInfoWidget::Construct(const FArguments& InArgs)
{
	// ��ȡGameStyle����ͼ��ʽ
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	
	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(400.f)
			.HeightOverride(100.f)
			[
				SNew(SBorder)
					.BorderImage(&GameStyle->RayInfoBrush)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(RayInfoTextBlock, STextBlock)
							.Font(GameStyle->Font_Outline_50)
							.ColorAndOpacity(GameStyle->FontColor_Black)
					]
			]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDRayInfoWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// ��һ֡��ʱ����г�ʼ��
	if (!IsInitRayInfoEvent) {
		RegisterRayInfoEvent.ExecuteIfBound(RayInfoTextBlock);		// ����ί�а󶨱���
		IsInitRayInfoEvent = true;
	}
}
