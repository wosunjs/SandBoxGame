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
	// 获取GameStyle的蓝图样式
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");
	// 获取DPIScaler
	UIScaler = InArgs._UIScaler;

	ChildSlot
		[
			// 全屏幕填充背包界面
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
								// 背景图
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SImage)
										.Image(&GameStyle->PackageBGBrush)
								]

								// 底部快捷栏
								+SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(40.f, 680.f, 40.f, 40.f))
								[
									SAssignNew(ShortcutGrid, SUniformGridPanel)
								]
								
								// 背包主体
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(40.f, 320.f, 40.f, 160.f))
								[
									SAssignNew(PackageGrid, SUniformGridPanel)
								]

								// 合成表输入框
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(80.f, 40.f, 480.f, 520.f))
								[
									SAssignNew(CompoundGrid, SUniformGridPanel)
								]

								// 合成表输出框
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.Padding(FMargin(560.f, 120.f, 160.f, 600.f))
								[
									SAssignNew(OutputBorder, SBorder)
								]

								// 合成小箭头
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
	// 如果背包显示并且世界存在，实时更新鼠标位置
	if (GetVisibility() == EVisibility::Visible && GEngine)
	{
		GEngine->GameViewport->GetMousePosition(MousePosition);
		// Debug 用，以此显示鼠标的绝对坐标和经过 DPI 值适配后的相对坐标，测试完可以删掉
		SDHelper::Debug(FString("AbsoMousePos : ") + MousePosition.ToString(), 0.f);
		MousePosition = MousePosition / UIScaler.Get();
		// Debug 用
		SDHelper::Debug(FString("RelaMousePos : ") + MousePosition.ToString(), 0.f);
	}

	// 如果背包管理器已经初始化
	if (IsInitPackageManager) {
		// 实时更新容器悬停显示
		SDPackageManager::Get()->UpdateHovered(MousePosition, AllottedGeometry);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDPackageWidget::InitPackageManager()
{
	// 初始化快捷栏
	for (int i = 0; i < 9; ++i) {
		// 创建容器实例
		TSharedPtr<SSDContainerBaseWidget> NewContainer = 
			SSDContainerBaseWidget::CreateContainer(EContainerType::Shortcut, i);
		// 将容器添加到UI
		ShortcutGrid->AddSlot(i, 0)[NewContainer->AsShared()];
		// 将容器注册到背包管理器
		SDPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Shortcut);
	}

	// 初始化背包主体
	for (int i = 0; i < 36; ++i) {
		TSharedPtr<SSDContainerBaseWidget> NewContainer =
			SSDContainerBaseWidget::CreateContainer(EContainerType::Normal, i);
		PackageGrid->AddSlot(i % 9, i / 9)[NewContainer->AsShared()];
		SDPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Normal);
	}

	// 初始化合成台
	for (int i = 0; i < 9; ++i) {
		TSharedPtr<SSDContainerBaseWidget> NewContainer =
			SSDContainerBaseWidget::CreateContainer(EContainerType::Input, i);
		CompoundGrid->AddSlot(i % 3, i / 3)[NewContainer->AsShared()];
		SDPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Input);
	}

	// 初始化输出容器
	TSharedPtr<SSDContainerBaseWidget> NewContainer =
		SSDContainerBaseWidget::CreateContainer(EContainerType::Output, 1);
	OutputBorder->SetContent(NewContainer->AsShared());
	SDPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Output);

	// 初始化完成背包管理器
	IsInitPackageManager = true;

	// 初始化给予1把武器3个苹果
	SDPackageManager::Get()->AddObject(7);
	SDPackageManager::Get()->AddObject(3);
	SDPackageManager::Get()->AddObject(3);
	SDPackageManager::Get()->AddObject(3);
}

int32 SSDPackageWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, 
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, 
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// 调用父类实现
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, 
		InWidgetStyle, bParentEnabled);

	// 检测背包管理器是否初始化
	if (!IsInitPackageManager) {
		return LayerId;
	}

	if (GetVisibility() == EVisibility::Visible && SDPackageManager::Get()->ObjectIndex != 0 
		&& SDPackageManager::Get()->ObjectNum > 0) {
		// 渲染物品图标
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId + 120,	// 层次浮于上层
			AllottedGeometry.ToPaintGeometry(MousePosition - FVector2D(32.f,32.f), FVector2D(64.f,64.f)),	// 位置
			SDDataHandle::Get()->ObjectBrushList[SDPackageManager::Get()->ObjectIndex],		// 笔刷
			ESlateDrawEffect::None,
			FLinearColor(1.f,1.f,1.f,1.f)
		);

		// 如果物体数量不为1则绘制数量
		if (SDPackageManager::Get()->ObjectNum != 1) {
			FSlateDrawElement::MakeText(
				OutDrawElements,
				LayerId + 120,
				AllottedGeometry.ToPaintGeometry(MousePosition + FVector2D(12.f, 16.f), FVector2D(16.f, 16.f)),
				FString::FromInt(SDPackageManager::Get()->ObjectNum),	// 绘制内容
				GameStyle->Font_Outline_16,		// 字体大小
				ESlateDrawEffect::None,			// 绘制特效
				GameStyle->FontColor_Black		// 字体颜色
			);
		} 
	}

	return LayerId;
}

FReply SSDPackageWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// 如果背包管理器还没有初始化
	if (!IsInitPackageManager) {
		return FReply::Handled();
	}

	// 如果是左键点击
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		SDPackageManager::Get()->LeftOption(MousePosition, MyGeometry);
	}
	// 如果是右键
	if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
		SDPackageManager::Get()->RightOption(MousePosition, MyGeometry);
	}

	return FReply::Handled();
}
