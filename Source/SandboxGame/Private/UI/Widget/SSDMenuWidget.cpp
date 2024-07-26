// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDMenuWidget.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "UI/Widget/SSDMenuIteamWidget.h"
#include "Data/SDTypes.h"
#include "Data/SDDataHandle.h"
#include "UI/Widget/SSDGameOptionWidget.h"
#include "UI/Widget/SSDNewGameWidget.h"
#include "UI/Widget/SSDChooseRecordWidget.h"
#include "GamePlay/SDMenuController.h"
#include "Common/SDHelper.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Actor.h"

struct MenuGroup
{
	// �˵�����
	FText MenuName;
	// �˵��߶�
	float MenuHeight;
	// �����ӿؼ�����
	TArray<TSharedPtr<SCompoundWidget>> ChildWidget;
	// ���캯��
	MenuGroup(const FText _MenuName, const float _MenuHeight, const TArray<TSharedPtr<SCompoundWidget>>* _ChildWidget)
		: MenuName(_MenuName), MenuHeight(_MenuHeight), ChildWidget(*_ChildWidget){}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDMenuWidget::Construct(const FArguments& InArgs)
{
	// ͨ��SDStyle��ȡSDMenuWidgetStyle�ڱ༭���е�MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// ���ű�������
	FSlateApplication::Get().PlaySound(MenuStyle->MenuBackgroundMusic);

	ChildSlot
	[
		SAssignNew(RootSizeBox, SBox)	// SBox����Slot��ֻ�ܲ���һ�������
		[
			SNew(SOverlay)
				+ SOverlay::Slot()	// �˵���
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.0f, 50.0f, 0.0f, 0.0f))	// �Ϸ�Ԥ��50�ռ�ű���
					[
						SNew(SImage).Image(&MenuStyle->MenuBackgroundBrush)
					]

				+ SOverlay::Slot()	// �˵������ͼ��
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					.Padding(FMargin(0.0f, 25.0f, 0.0f, 0.0f))	// �·�Ԥ��25�ռ�
					[
						SNew(SImage).Image(&MenuStyle->LeftIconBrush)
					]

				+ SOverlay::Slot()	// �˵����Ҳ�ͼ��
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					.Padding(FMargin(0.0f, 25.0f, 0.0f, 0.0f))	// �·�Ԥ��25�ռ�
					[
						SNew(SImage).Image(&MenuStyle->RightIconBrush)
					]

				+ SOverlay::Slot()	// �˵�����
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					[
						SNew(SBox)
							.WidthOverride(400.0f)
							.HeightOverride(100.0f)
							[
								SNew(SBorder)
									.BorderImage(&MenuStyle->TitleBorderBrush)
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									//.Padding(FMargin(0.0f, 0.0f, 0.0f, 10.0f))	// �·�Ԥ��25�ռ�
									[
										SAssignNew(TitleText, STextBlock)
											.Font(MenuStyle->Font_50)	// ��������
											.Text(NSLOCTEXT("SDMenu", "Menu", "Menu"))
									]
							]
					]
				+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.Padding(FMargin(0.0f, 130.0f, 0.0f, 0.0f))	// �·�Ԥ��130�ռ�
					[
						SAssignNew(ContentBox, SVerticalBox)	// ����SVerticalBox����ȡָ��ContentBox
					]


		]
	];
	
	InitializeMenuList();

	InitializedAnimation();
	
}

void SSDMenuWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	switch (AnimState)
	{
	case EMenuAnim::Stop:
		break;
	case EMenuAnim::Close:
		// ������ڲ���
		if (MenuAnimation.IsPlaying()) {
			// ʵʱ�޸�Menu�Ĵ�С
			ResetWidgetSize(MenuCurve.GetLerp() * 600.f, -1.f);

			// �ڹرն���������40%����ʾ�����ʱ�����ò���ʾ���
			if (MenuCurve.GetLerp() < 0.6f && IsMenuShow) {
				ChooseWidget(EMenuType::None);
			}
		}
		else {
			// �رն������Ž���������״̬Ϊ��
			AnimState = EMenuAnim::Open;

			// ��ʼ���Ŵ򿪶���
			MenuAnimation.Play(this->AsShared());
		}
		break;
	case EMenuAnim::Open:
		// ������ڲ���
		if (MenuAnimation.IsPlaying()) {
			// ʵʱ�޸�Menu�Ĵ�С
			ResetWidgetSize(MenuCurve.GetLerp() * 600.f, CurrentHeight);

			// �ڴ򿪶���������60%��û����ʾ���ʱ��ʾ���
			if (MenuCurve.GetLerp() > 0.6f && !IsMenuShow) {
				ChooseWidget(CurrentMenu);
			}
		}
		else {
			// �رն������Ž���������״̬Ϊ��
			AnimState = EMenuAnim::Open;

			// ������ť
			ControlLocked = false;
		}
		break;
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDMenuWidget::MenuIteamOnclicked(EMenuItem::Type ItemType)
{
	// ����˵��������򲻿ɵ����ť��ֱ�ӷ���
	if (ControlLocked) return;

	// ��ס��ť
	ControlLocked = true;

	switch (ItemType)
	{
	case EMenuItem::None:
		break;
	case EMenuItem::StartGame:
		PlayClose(EMenuType::StartGame);
		break;
	case EMenuItem::GameOption:
		PlayClose(EMenuType::GameOption);
		break;
	case EMenuItem::QuitGame:
		// �������֣���Ϻ�ر�
		SDHelper::PlayerSoundAndCall(GWorld, MenuStyle->ExitGameSound, this, &SSDMenuWidget::QuitGame);
		
		ControlLocked = false;
		break;
	case EMenuItem::NewGame:
		PlayClose(EMenuType::NewGame);
		break;
	case EMenuItem::LoadRecord:
		PlayClose(EMenuType::ChooseRecord);
		break;
	case EMenuItem::StartGameGoBack:
		PlayClose(EMenuType::MainMenu);
		break;
	case EMenuItem::GameOptionGoBack:
		PlayClose(EMenuType::MainMenu);
		break;
	case EMenuItem::NewGameGoBack:
		PlayClose(EMenuType::StartGame);
		break;
	case EMenuItem::ChooseRecordGoBack:
		PlayClose(EMenuType::StartGame);
		break;
	case EMenuItem::EnterGame:
		// ����Ƿ�(�´浵���Ϸ�)���Խ�����Ϸ
		if (NewGameWidget->AllowEnterGame()) {
			// �������֣�������Ϸ
			SDHelper::PlayerSoundAndCall(GWorld, MenuStyle->StartGameSound, this, &SSDMenuWidget::EnterGame);
		}
		else {
			// ��ť����
			ControlLocked = false;
		}
		
		break;
	case EMenuItem::EnterRecord:
		// ѡ��浵��������޸Ĵ浵��������ֱ�ӽ����޸Ĳ����Ա���
		ChooseRecordWidget->UpdateRecordName();

		SDHelper::PlayerSoundAndCall(GWorld, MenuStyle->StartGameSound, this, &SSDMenuWidget::EnterGame);
		break;
	}
}

void SSDMenuWidget::ChangeCulture(ECultureTeam Culture)
{
	SDDataHandle::Get()->ChangeLocalizationCulture(Culture);
}

void SSDMenuWidget::ChangeVolume(const float MusicVolume, const float SoundVolume)
{
	SDDataHandle::Get()->ChangeVolume(MusicVolume, SoundVolume);
}



void SSDMenuWidget::InitializeMenuList()
{
	// ������
	TArray<TSharedPtr<SCompoundWidget>> MainMenuList;
	// ��Ϸ��ʼ��ť
	MainMenuList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "StartGame", "StartGame"))
		.ItemType(EMenuItem::StartGame).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// ��Ϸ���ð�ť
	MainMenuList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GameOption", "GameOption"))
		.ItemType(EMenuItem::GameOption).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// ��Ϸ�˳���ť
	MainMenuList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "QuitGame", "QuitGame"))
		.ItemType(EMenuItem::QuitGame).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// �������map������
	MenuMap.Add(EMenuType::MainMenu, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "Menu", "Menu")
		, 510.f, &MainMenuList)));

	// ��ʼ��Ϸ����
	TArray<TSharedPtr<SCompoundWidget>> StartGameList;
	// �½���Ϸ��ť
	StartGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "NewGame", "NewGame"))
		.ItemType(EMenuItem::NewGame).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// ���ش浵��ť
	StartGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "LoadRecord", "LoadRecord"))
		.ItemType(EMenuItem::LoadRecord).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// ���������水ť
	StartGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GoBack", "GoBack"))
		.ItemType(EMenuItem::StartGameGoBack).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// �������map������
	MenuMap.Add(EMenuType::StartGame, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "StartGame", "StartGame")
		, 510.f, &StartGameList)));

	// ��Ϸ���ý���
	TArray<TSharedPtr<SCompoundWidget>> GameOptionList;
	// ʵ������Ϸ���õ�Widget
	SAssignNew(GameOptionWidget, SSDGameOptionWidget)
		.ChangeCulture(this, &SSDMenuWidget::ChangeCulture)
		.ChangeVolume(this, &SSDMenuWidget::ChangeVolume);
	// ��ӿؼ�������
	GameOptionList.Add(GameOptionWidget);
	GameOptionList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GoBack", "GoBack"))
		.ItemType(EMenuItem::GameOptionGoBack).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// �������map������
	MenuMap.Add(EMenuType::GameOption, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "GameOption", "GameOption")
		, 610.f, &GameOptionList)));

	// ��ʼ����Ϸ����
	TArray<TSharedPtr<SCompoundWidget>> NewGameList;
	SAssignNew(NewGameWidget, SSDNewGameWidget);
	NewGameList.Add(NewGameWidget);
	// ������Ϸ��ť
	NewGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "EnterGame", "EnterGame"))
		.ItemType(EMenuItem::EnterGame).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// ���ذ�ť
	NewGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GoBack", "GoBack"))
		.ItemType(EMenuItem::NewGameGoBack).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// �������map������
	MenuMap.Add(EMenuType::NewGame, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "NewGame", "NewGame")
		, 510.f, &NewGameList)));

	// ѡ��浵����
	TArray<TSharedPtr<SCompoundWidget>> ChooseRecordList;
	SAssignNew(ChooseRecordWidget, SSDChooseRecordWidget);
	ChooseRecordList.Add(ChooseRecordWidget);
	// ����浵��ť
	ChooseRecordList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "EnterRecord", "EnterRecord"))
		.ItemType(EMenuItem::EnterRecord).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// ���ذ�ť
	ChooseRecordList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GoBack", "GoBack"))
		.ItemType(EMenuItem::ChooseRecordGoBack).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// �������map������
	MenuMap.Add(EMenuType::ChooseRecord, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "LoadRecord", "LoadRecord")
		, 510.f, &ChooseRecordList)));

}

void SSDMenuWidget::ChooseWidget(EMenuType::Type WidgetType)
{
	// �����Ƿ��Ѿ���ʾ�˵�
	IsMenuShow = WidgetType != EMenuType::None;

	// �ṩMenuMapѡ��Ҫ��ʾ�Ľ���
	// �Ƴ��������
	ContentBox->ClearChildren();
	// ���Menutype��Noneֱ�ӷ���
	if (WidgetType == EMenuType::None) {
		return;
	}
	// ѭ�����ָ����������
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It((*MenuMap.Find(WidgetType))->ChildWidget); It; ++It) {
		ContentBox->AddSlot()
			.AutoHeight()
			[(*It)->AsShared()];
	}
	// ���ı���
	TitleText->SetText((*MenuMap.Find(WidgetType))->MenuName);
	
}

// Ĭ�ϸ߶Ȳ��޸�
void SSDMenuWidget::ResetWidgetSize(float NewWidget, float NewHeight = 0.f)
{
	RootSizeBox->SetWidthOverride(NewWidget);
	if (NewHeight > 0.f) {
		RootSizeBox->SetHeightOverride(NewHeight);
	}
}

void SSDMenuWidget::InitializedAnimation()
{
	// ��ʼ��ʱ
	const float StartDelay = 0.2f;
	// ����ʱ��
	const float AnimDuration = 0.4f;

	// ʵ��������������
	MenuAnimation = FCurveSequence();
	// ʵ�������߾����������涯��ʵʱ�仯(��0��1)
	MenuCurve = MenuAnimation.AddCurve(StartDelay, AnimDuration, ECurveEaseFunction::QuadInOut);

	// ��ʼ����Menu��С
	ResetWidgetSize(600.f, 510.f);

	// ѡ��������
	ChooseWidget(EMenuType::MainMenu);

	// ��ť�ɵ�������Ŷ���ʱ��ס��
	ControlLocked = false;

	// ���ö���״̬Ϊֹͣ
	AnimState = EMenuAnim::Stop;

	// ���ö���������������β=1
	MenuAnimation.JumpToEnd();

}

void SSDMenuWidget::PlayClose(EMenuType::Type NewMenu)
{
	// �����µĽ���
	CurrentMenu = NewMenu;

	// �����¸߶�
	CurrentHeight = (*MenuMap.Find(NewMenu))->MenuHeight;

	// ���ò���״̬ΪClose
	AnimState = EMenuAnim::Close;

	// ���ŷ��򶯻�
	MenuAnimation.PlayReverse(this->AsShared());

	// �����л��˵�����
	FSlateApplication::Get().PlaySound(MenuStyle->MenuItemChangeSound);

}

void SSDMenuWidget::QuitGame()
{
	// ͨ��PlayerController��ȡ
	Cast<ASDMenuController>(UGameplayStatics::GetPlayerController(GWorld, 0))->ConsoleCommand("quit");

}

void SSDMenuWidget::EnterGame()
{
	UGameplayStatics::OpenLevel(GWorld, FName("GameMap"));

	// ���������ֺ�ť����
	ControlLocked = false;
}
