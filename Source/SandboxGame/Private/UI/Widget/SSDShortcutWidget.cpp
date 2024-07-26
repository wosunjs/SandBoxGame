// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDShortcutWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Data/SDDataHandle.h"



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDShortcutWidget::Construct(const FArguments& InArgs)
{
	// ��ȡGameStyle����ͼ��ʽ
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(900.f)
			.HeightOverride(160.f)
			[
				SNew(SOverlay)
					// ��Ʒ��Ϣ����
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					[
						SAssignNew(shortcutInfoTextBlock, STextBlock)
							.Font(GameStyle->Font_Outline_40)
							.ColorAndOpacity(GameStyle->FontColor_White)
					]
					// ��Ʒ��
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.f, 60.f, 0.f, 0.f))
					[
						SAssignNew(GridPanel, SUniformGridPanel)

					]
			]
	];

	// ��������δ��ʼ��
	IsInitContainer = false;

	
}

void SSDShortcutWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!IsInitContainer) {
		InitContainer();
		IsInitContainer = true;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SSDShortcutWidget::InitContainer()
{
	TArray<TSharedPtr<ShortcutContainer>> ContainerList;	// �ɵ���������ɵ��б�

	// �����б��ʼ��
	for (int i = 0; i < 9; i++) {
		// ��������
		TSharedPtr<SBorder> ContainerBorder;

		// ����UI���
		TSharedPtr<SBorder> ObjectImage;		// ��ƷͼƬ
		TSharedPtr<STextBlock> ObjectNumText;	// ��Ʒ����
		SAssignNew(ContainerBorder, SBorder)
			.Padding(FMargin(10.f))
			[
				SAssignNew(ObjectImage, SBorder)
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(0.f, 0.f, 5.f, 0.f))
					[
						SAssignNew(ObjectNumText, STextBlock)
							.Font(GameStyle->Font_Outline_20)
							.ColorAndOpacity(GameStyle->FontColor_Black)
					]
			];

		// ÿ��ʼ��һ�����ContainerBorder�ͽ�����ӵ�GridPanel
		GridPanel->AddSlot(i, 0)
		[
			ContainerBorder->AsShared()
		] ;

		// �������������Ʒ��ֵ��ʵ����һ�������ṹ�壩
		TSharedPtr<ShortcutContainer> Container = MakeShareable(new ShortcutContainer(ContainerBorder
			,ObjectImage , ObjectNumText, &GameStyle->NormalContainerBrush, &GameStyle->ChoosedContainerBrush, 
			&SDDataHandle::Get()->ObjectBrushList));

		// �������һ��Ϊѡ��
		if (i == 0) Container->SetChoosed(true);

		// ��ӵ�����
		ContainerList.Add(Container);
	}

	// ��ʵ�����Ľṹ��ע���PlayerState����������
	RegisterShortcutContainer.ExecuteIfBound(&ContainerList, shortcutInfoTextBlock);
	

}

