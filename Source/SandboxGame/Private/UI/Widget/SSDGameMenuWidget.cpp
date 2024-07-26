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
	// ��ȡGameStyle����ͼ��ʽ
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
	// ���˵�������ť
	MenuItemList.Add(
		// ��һ��Ϊ������Ϸѡ�ť
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
		// �ڶ���Ϊ������Ϸ�İ�ť
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
		// ������Ϊ�˳���Ϸ�İ�ť
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

	// ��Ϸ���ò˵�
	OptionItemList.Add(
		SNew(SSDGameOptionWidget)
		.ChangeCulture(this, &SSDGameMenuWidget::ChangeCulture)
		.ChangeVolume(this, &SSDGameMenuWidget::ChangeVolume)
	);
	// ���ذ�ť
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

	// ��Ϸ�˳���ť
	SAssignNew(QuitGameButton, SButton)
		.OnClicked(this, &SSDGameMenuWidget::QuitGameEvent)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(NSLOCTEXT("SDGame", "QuitGame", "QuitGame"))
				.Font(GameStyle->Font_30)
		];

	// ���˵���ť��䵽UI
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
	// ������Ϸ�˵�
	// ����������
	VertBox->ClearChildren();
	// �����ÿؼ����
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(3.2f)
		[
			OptionItemList[0]->AsShared()
		];
	// �����ذ�ť���
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			OptionItemList[1]->AsShared()
		];
	// ���ø߶�
	RootBox->SetHeightOverride(520.f);

	return FReply::Handled();
}

FReply SSDGameMenuWidget::SaveGameEvent()
{
	// ִ��ί�У�ʵ�ֱ���
	SaveGameDele.ExecuteIfBound();
	// ���ð�ť������
	SaveGameButton->SetVisibility(EVisibility::HitTestInvisible);
	// �޸İ�ť����Ϊ�������
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
	// ���UI
	VertBox->ClearChildren();
	// ���˵���ť���
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
	// ���ø߶�
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
	// ������а�ť
	VertBox->ClearChildren();
	// ��ʾ�˳���Ϸ��ť
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			QuitGameButton->AsShared()
		];

	// ���ø߶�
	RootBox->SetHeightOverride(200.f);
}

void SSDGameMenuWidget::ResetMenu()
{
	// ������пؼ�
	VertBox->ClearChildren();
	// ���˵���ť���
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
	// ���ø߶�
	RootBox->SetHeightOverride(400.f);

	// ���ô浵��ť�ɼ�
	SaveGameButton->SetVisibility(EVisibility::Visible);
	// �޸Ĵ浵��ť��ʾ����
	SaveGameText->SetText(NSLOCTEXT("SDGame", "SaveGame", "SaveGame"));
}
