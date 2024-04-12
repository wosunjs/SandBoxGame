// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDMenuIteamWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDMenuIteamWidget::Construct(const FArguments& InArgs)
{
	// ͨ��SDStyle��ȡSDMenuWidgetStyle�ڱ༭���е�MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	Onclicked = InArgs._Onclicked;	// ��ȡ��ʼ�����OnClick��Ӧ��������SSDMenuWidget����ɸ�ֵ��

	ItemType = InArgs._ItemType.Get();	// ��ȡ���ı�����Ҫʹ��.Get()����

	
	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(500.0f)
			.HeightOverride(100.0f)
			[	
				// ����ͼƬ
				SNew(SOverlay)
					+ SOverlay::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					[
						SNew(SImage)
							.Image(&MenuStyle->MenuItemBrush)
							// ������ͼƬ����ɫ�󶨵�GetTintColor����������������ʱ�ı���ɫ
							.ColorAndOpacity(this, &SSDMenuIteamWidget::GetTintColor)
					]
				// ��ť������
					+ SOverlay::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
							.Text(InArgs._ItemText)
							.Font(MenuStyle->Font_60)
					]
			]
	];

	// ��ʼ����Ա����
	IsMouseButtonDown = false;
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SSDMenuIteamWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	IsMouseButtonDown = true;
	return FReply::Handled();
}

FReply SSDMenuIteamWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// ���̧�𴥷�ί��
	if (IsMouseButtonDown) {
		IsMouseButtonDown = false;
		Onclicked.ExecuteIfBound(ItemType);	// ί��ִ��
	}

	return FReply::Handled();
}

void SSDMenuIteamWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	IsMouseButtonDown = false;
}

FSlateColor SSDMenuIteamWidget::GetTintColor() const
{
	if (IsMouseButtonDown) return FLinearColor(1.0f, 0.1f, 0.1f, 0.5f);
	return FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
}
