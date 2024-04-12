// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDMenuHUDWidget.h"
#include "UI/Widget/SSDMenuWidget.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SDPIScaler.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDMenuHUDWidget::Construct(const FArguments& InArgs)
{
	// ͨ��SDStyle��ȡSDMenuWidgetStyle�ڱ༭���е�MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// �󶨿ؼ����Ź��򷽷�
	UIScaler.Bind(this, &SSDMenuHUDWidget::GetUIScaler);
	
	ChildSlot
	[
		SNew(SDPIScaler)		// DPI���ڴ�С�޸�ʱ����UIScaler
			.DPIScale(UIScaler)
			[
			SNew(SOverlay) 
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)	// ��ʽHorizontalˮƽȫ���
				.VAlign(VAlign_Fill)	// ��ʽVertical��ֱȫ���
					[
						// ��ӱ���ͼƬ:��ʵ����һ�������Ȼ���������һ����ˢ
						SNew(SImage).Image(&MenuStyle->MenuHUDBackgroundBrush)
					]
			
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)	// ��ʽˮƽ����
				.VAlign(VAlign_Center)	// ��ʽ��ֱ����
				.Expose(ImageSlot)
					[
						// ʵ����SSDMenuWidget����ȡ��ָ��MenuWidget
						SAssignNew(MenuWidget, SSDMenuWidget)
					]

			
			]
	];
	
}

float SSDMenuHUDWidget::GetUIScaler() const
{
	// ���ڿ�ȴ���1080ʱ����ֵΪ1��С��1080ʱ���Ա仯
	auto VSW = GetViewportSize().Y;
	if (VSW < 1080.0f) {
		return VSW / 1080.0f;
	}
	return 1.0f;
}

FVector2D SSDMenuHUDWidget::GetViewportSize() const
{
	FVector2D Res(1920, 1080);
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(Res);
	}
	return Res;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
