// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDGameMenuWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "UI/Widget/SSDGameOptionWidget.h"

#include "Engine//World.h"
#include "Data/SDDataHandle.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SDPlayerController.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDGameMenuWidget::Construct(const FArguments& InArgs)
{
	// 获取GameStyle的蓝图样式
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	ChildSlot
		[
			SAssignNew(RootBox, SBox)
				.WidthOverride(600.f)
				.HeightOverride(400.f)
				.Padding(FMargin(50.f))
				[
					SAssignNew(VertBox, SVerticalBox)
				]
		];

	InitializeWidget();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDGameMenuWidget::InitializeWidget()
{
	// 主菜单三个按钮
	MenuItemList.Add(
		// 第一个为进入游戏选项按钮
		SNew(SButton)
		.OnClicked(this, &SSDGameMenuWidget::OptionEvent)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(NSLOCTEXT("SDGame", "GameOption", "GameOption"))
				.Font(GameStyle->Font_30)
		]
	);
	MenuItemList.Add(
		// 第二个为保存游戏的按钮
		SAssignNew(SaveGameButton, SButton)
		.OnClicked(this, &SSDGameMenuWidget::SaveGameEvent)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(SaveGameText, STextBlock)
				.Text(NSLOCTEXT("SDGame", "SaveGame", "SaveGame"))
				.Font(GameStyle->Font_30)
		]
	);
	MenuItemList.Add(
		// 第三个为退出游戏的按钮
		SNew(SButton)
		.OnClicked(this, &SSDGameMenuWidget::QuitGameEvent)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(NSLOCTEXT("SDGame", "QuitGame", "QuitGame"))
				.Font(GameStyle->Font_30)
		]
	);

	// 游戏设置菜单
	OptionItemList.Add(
		SNew(SSDGameOptionWidget)
		.ChangeCulture(this, &SSDGameMenuWidget::ChangeCulture)
		.ChangeVolume(this, &SSDGameMenuWidget::ChangeVolume)
	);
	// 返回按钮
	OptionItemList.Add(
		SNew(SButton)
		.OnClicked(this, &SSDGameMenuWidget::GoBackEvent)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(NSLOCTEXT("SDGame", "GoBack", "GoBack"))
				.Font(GameStyle->Font_30)
		]
	);

	// 游戏退出按钮
	SAssignNew(QuitGameButton, SButton)
		.OnClicked(this, &SSDGameMenuWidget::QuitGameEvent)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(NSLOCTEXT("SDGame", "QuitGame", "QuitGame"))
				.Font(GameStyle->Font_30)
		];

	// 将菜单按钮填充到UI
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It) {
		VertBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(10.f)
			.FillHeight(1.f)
			[
				(*It)->AsShared()
			];
	}
	
}

FReply SSDGameMenuWidget::OptionEvent()
{
	// 进入游戏菜单
	// 清空所有组件
	VertBox->ClearChildren();
	// 将设置控件填充
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(3.2f)
		[
			OptionItemList[0]->AsShared()
		];
	// 将返回按钮填充
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			OptionItemList[1]->AsShared()
		];
	// 设置高度
	RootBox->SetHeightOverride(520.f);

	return FReply::Handled();
}

FReply SSDGameMenuWidget::SaveGameEvent()
{
	// 执行委托，实现保存
	SaveGameDele.ExecuteIfBound();
	// 设置按钮不可用
	SaveGameButton->SetVisibility(EVisibility::HitTestInvisible);
	// 修改按钮文字为保存完毕
	SaveGameText->SetText(NSLOCTEXT("SDGame", "SaveCompleted", "SaveCompleted"));
	
	return FReply::Handled();
}

FReply SSDGameMenuWidget::QuitGameEvent()
{
	Cast<ASDPlayerController>(UGameplayStatics::GetPlayerController(GWorld, 0))->ConsoleCommand("quit");
	return FReply::Handled();
}

FReply SSDGameMenuWidget::GoBackEvent()
{
	// 清空UI
	VertBox->ClearChildren();
	// 将菜单按钮填充
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It) {
		VertBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(10.f)
			.FillHeight(1.f)
			[
				(*It)->AsShared()
			];
	}
	// 设置高度
	RootBox->SetHeightOverride(400.f);
	return FReply::Handled();
}

void SSDGameMenuWidget::ChangeCulture(ECultureTeam Culture)
{
	SDDataHandle::Get()->ChangeLocalizationCulture(Culture);
}

void SSDGameMenuWidget::ChangeVolume(const float MusicVolume, const float SoundVolume)
{
	
}

void SSDGameMenuWidget::GameLose()
{
	// 清空所有按钮
	VertBox->ClearChildren();
	// 显示退出游戏按钮
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			QuitGameButton->AsShared()
		];

	// 设置高度
	RootBox->SetHeightOverride(200.f);
}

void SSDGameMenuWidget::ResetMenu()
{
	// 清空所有控件
	VertBox->ClearChildren();
	// 将菜单按钮填充
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It) {
		VertBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(10.f)
			.FillHeight(1.f)
			[
				(*It)->AsShared()
			];
	}
	// 设置高度
	RootBox->SetHeightOverride(400.f);

	// 重置存档按钮可见
	SaveGameButton->SetVisibility(EVisibility::Visible);
	// 修改存档按钮显示文字
	SaveGameText->SetText(NSLOCTEXT("SDGame", "SaveGame", "SaveGame"));
}
