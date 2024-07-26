// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Package/SSDPackageWidget.h"
#include "SlateOptMacros.h"

#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "UI/Widget/Package/SSDContainerBaseWidget.h"

#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "Engine/GameEngine.h"
#include "Engine/Engine.h"
#include "Common/SDHelper.h"
#include "Data/SDDataHandle.h"

#include "Player/SDPackageManager.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDPackageWidget::Construct(const FArguments& InArgs)
{
	// ��ȡGameStyle����ͼ��ʽ
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");
	// ��ȡDPIScaler
	UIScaler = InArgs._UIScaler;

	ChildSlot
		[
			// ȫ��Ļ��䱳������
			SNew(SOverlay)
				+SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.Padding(FMargin(0.f, 0.f, 50.f, 0.f)) 
				[
					SNew(SBox)
						.WidthOverride(800.f)
						.HeightOverride(800.f)
						[
							SNew(SOverlay)
								// ����ͼ
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SImage)
										.Image(&GameStyle->PackageBGBrush)
								]

								// �ײ������
								+SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(40.f, 680.f, 40.f, 40.f))
								[
									SAssignNew(ShortcutGrid, SUniformGridPanel)
								]
								
								// ��������
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(40.f, 320.f, 40.f, 160.f))
								[
									SAssignNew(PackageGrid, SUniformGridPanel)
								]

								// �ϳɱ������
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(80.f, 40.f, 480.f, 520.f))
								[
									SAssignNew(CompoundGrid, SUniformGridPanel)
								]

								// �ϳɱ������
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(560.f, 120.f, 160.f, 600.f))
								[
									SAssignNew(OutputBorder, SBorder)
								]

								// �ϳ�С��ͷ
								+SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(400.f, 120.f, 320.f, 600.f))
								[
									SNew(SImage)
									.Image(&GameStyle->CompoundArrowBrush)
								]
								
						]
				]

		];

	MousePosition = FVector2D(0.f, 0.f);

	IsInitPackageManager = false;

}

void SSDPackageWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// ���������ʾ����������ڣ�ʵʱ�������λ��
	if (GetVisibility() == EVisibility::Visible && GEngine)
	{
		GEngine->GameViewport->GetMousePosition(MousePosition);
		// Debug �ã��Դ���ʾ���ľ�������;��� DPI ֵ������������꣬���������ɾ��
		SDHelper::Debug(FString("AbsoMousePos : ") + MousePosition.ToString(), 0.f);
		MousePosition = MousePosition / UIScaler.Get();
		// Debug ��
		SDHelper::Debug(FString("RelaMousePos : ") + MousePosition.ToString(), 0.f);
	}

	// ��������������Ѿ���ʼ��
	if (IsInitPackageManager) {
		// ʵʱ����������ͣ��ʾ
		SDPackageManager::Get()->UpdateHovered(MousePosition, AllottedGeometry);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDPackageWidget::InitPackageManager()
{
	// ��ʼ�������
	for (int i = 0; i < 9; ++i) {
		// ��������ʵ��
		TSharedPtr<SSDContainerBaseWidget> NewContainer = 
			SSDContainerBaseWidget::CreateContainer(EContainerType::Shortcut, i);
		// ��������ӵ�UI
		ShortcutGrid->AddSlot(i, 0)[NewContainer->AsShared()];
		// ������ע�ᵽ����������
		SDPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Shortcut);
	}

	// ��ʼ����������
	for (int i = 0; i < 36; ++i) {
		TSharedPtr<SSDContainerBaseWidget> NewContainer =
			SSDContainerBaseWidget::CreateContainer(EContainerType::Normal, i);
		PackageGrid->AddSlot(i % 9, i / 9)[NewContainer->AsShared()];
		SDPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Normal);
	}

	// ��ʼ���ϳ�̨
	for (int i = 0; i < 9; ++i) {
		TSharedPtr<SSDContainerBaseWidget> NewContainer =
			SSDContainerBaseWidget::CreateContainer(EContainerType::Input, i);
		CompoundGrid->AddSlot(i % 3, i / 3)[NewContainer->AsShared()];
		SDPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Input);
	}

	// ��ʼ���������
	TSharedPtr<SSDContainerBaseWidget> NewContainer =
		SSDContainerBaseWidget::CreateContainer(EContainerType::Output, 1);
	OutputBorder->SetContent(NewContainer->AsShared());
	SDPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Output);

	// ��ʼ����ɱ���������
	IsInitPackageManager = true;

	// ��ʼ������1������3��ƻ��
	SDPackageManager::Get()->AddObject(7);
	SDPackageManager::Get()->AddObject(3);
	SDPackageManager::Get()->AddObject(3);
	SDPackageManager::Get()->AddObject(3);
}

int32 SSDPackageWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, 
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, 
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// ���ø���ʵ��
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, 
		InWidgetStyle, bParentEnabled);

	// ��ⱳ���������Ƿ��ʼ��
	if (!IsInitPackageManager) {
		return LayerId;
	}

	if (GetVisibility() == EVisibility::Visible && SDPackageManager::Get()->ObjectIndex != 0 
		&& SDPackageManager::Get()->ObjectNum > 0) {
		// ��Ⱦ��Ʒͼ��
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId + 120,	// ��θ����ϲ�
			AllottedGeometry.ToPaintGeometry(MousePosition - FVector2D(32.f,32.f), FVector2D(64.f,64.f)),	// λ��
			SDDataHandle::Get()->ObjectBrushList[SDPackageManager::Get()->ObjectIndex],		// ��ˢ
			ESlateDrawEffect::None,
			FLinearColor(1.f,1.f,1.f,1.f)
		);

		// �������������Ϊ1���������
		if (SDPackageManager::Get()->ObjectNum != 1) {
			FSlateDrawElement::MakeText(
				OutDrawElements,
				LayerId + 120,
				AllottedGeometry.ToPaintGeometry(MousePosition + FVector2D(12.f, 16.f), FVector2D(16.f, 16.f)),
				FString::FromInt(SDPackageManager::Get()->ObjectNum),	// ��������
				GameStyle->Font_Outline_16,		// �����С
				ESlateDrawEffect::None,			// ������Ч
				GameStyle->FontColor_Black		// ������ɫ
			);
		} 
	}

	return LayerId;
}

FReply SSDPackageWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// ���������������û�г�ʼ��
	if (!IsInitPackageManager) {
		return FReply::Handled();
	}

	// �����������
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		SDPackageManager::Get()->LeftOption(MousePosition, MyGeometry);
	}
	// ������Ҽ�
	if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
		SDPackageManager::Get()->RightOption(MousePosition, MyGeometry);
	}

	return FReply::Handled();
}
