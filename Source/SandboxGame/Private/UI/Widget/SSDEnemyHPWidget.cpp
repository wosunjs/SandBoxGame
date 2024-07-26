// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDEnemyHPWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Notifications/SProgressBar.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDEnemyHPWidget::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SAssignNew(HPBar, SProgressBar)
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDEnemyHPWidget::ChangeHP(float HP)
{
	HP = FMath::Clamp<float>(HP, 0.f, 1.f);
	HPBar->SetPercent(HP);
	ResultColor = FLinearColor(1.f - HP, HP, 0.f, 1.f);		// ���HPΪ0����rgb(1,0,0)Ϊ��ɫ�����HPΪ1����RGB(0,1,0)Ϊ��ɫ
	HPBar->SetFillColorAndOpacity(FSlateColor(ResultColor));
}