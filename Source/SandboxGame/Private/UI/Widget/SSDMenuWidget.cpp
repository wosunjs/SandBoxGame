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
	// 菜单标题
	FText MenuName;
	// 菜单高度
	float MenuHeight;
	// 下属子控件数组
	TArray<TSharedPtr<SCompoundWidget>> ChildWidget;
	// 构造函数
	MenuGroup(const FText _MenuName, const float _MenuHeight, const TArray<TSharedPtr<SCompoundWidget>>* _ChildWidget)
		: MenuName(_MenuName), MenuHeight(_MenuHeight), ChildWidget(*_ChildWidget){}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDMenuWidget::Construct(const FArguments& InArgs)
{
	// 通过SDStyle获取SDMenuWidgetStyle在编辑器中的MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// 播放背景音乐
	FSlateApplication::Get().PlaySound(MenuStyle->MenuBackgroundMusic);

	ChildSlot
	[
		SAssignNew(RootSizeBox, SBox)	// SBox不能Slot，只能插入一个子组件
		[
			SNew(SOverlay)
				+ SOverlay::Slot()	// 菜单框
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.0f, 50.0f, 0.0f, 0.0f))	// 上方预留50空间放标题
					[
						SNew(SImage).Image(&MenuStyle->MenuBackgroundBrush)
					]

				+ SOverlay::Slot()	// 菜单框左侧图标
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					.Padding(FMargin(0.0f, 25.0f, 0.0f, 0.0f))	// 下方预留25空间
					[
						SNew(SImage).Image(&MenuStyle->LeftIconBrush)
					]

				+ SOverlay::Slot()	// 菜单框右侧图标
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					.Padding(FMargin(0.0f, 25.0f, 0.0f, 0.0f))	// 下方预留25空间
					[
						SNew(SImage).Image(&MenuStyle->RightIconBrush)
					]

				+ SOverlay::Slot()	// 菜单标题
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
									//.Padding(FMargin(0.0f, 0.0f, 0.0f, 10.0f))	// 下方预留25空间
									[
										SAssignNew(TitleText, STextBlock)
											.Font(MenuStyle->Font_50)	// 设置字体
											.Text(NSLOCTEXT("SDMenu", "Menu", "Menu"))
									]
							]
					]
				+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.Padding(FMargin(0.0f, 130.0f, 0.0f, 0.0f))	// 下方预留130空间
					[
						SAssignNew(ContentBox, SVerticalBox)	// 创建SVerticalBox并获取指针ContentBox
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
		// 如果正在播放
		if (MenuAnimation.IsPlaying()) {
			// 实时修改Menu的大小
			ResetWidgetSize(MenuCurve.GetLerp() * 600.f, -1.f);

			// 在关闭动画播放了40%且显示组件的时候设置不显示组件
			if (MenuCurve.GetLerp() < 0.6f && IsMenuShow) {
				ChooseWidget(EMenuType::None);
			}
		}
		else {
			// 关闭动画播放结束，设置状态为打开
			AnimState = EMenuAnim::Open;

			// 开始播放打开动画
			MenuAnimation.Play(this->AsShared());
		}
		break;
	case EMenuAnim::Open:
		// 如果正在播放
		if (MenuAnimation.IsPlaying()) {
			// 实时修改Menu的大小
			ResetWidgetSize(MenuCurve.GetLerp() * 600.f, CurrentHeight);

			// 在打开动画播放了60%且没有显示组件时显示组件
			if (MenuCurve.GetLerp() > 0.6f && !IsMenuShow) {
				ChooseWidget(CurrentMenu);
			}
		}
		else {
			// 关闭动画播放结束，设置状态为打开
			AnimState = EMenuAnim::Open;

			// 解锁按钮
			ControlLocked = false;
		}
		break;
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDMenuWidget::MenuIteamOnclicked(EMenuItem::Type ItemType)
{
	// 如果菜单已锁定则不可点击按钮，直接返回
	if (ControlLocked) return;

	// 锁住按钮
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
		// 播放音乐，完毕后关闭
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
		// 检测是否(新存档名合法)可以进入游戏
		if (NewGameWidget->AllowEnterGame()) {
			// 播放音乐，进入游戏
			SDHelper::PlayerSoundAndCall(GWorld, MenuStyle->StartGameSound, this, &SSDMenuWidget::EnterGame);
		}
		else {
			// 按钮解锁
			ControlLocked = false;
		}
		
		break;
	case EMenuItem::EnterRecord:
		// 选择存档后，如果有修改存档名，我们直接将其修改并加以保存
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
	// 主界面
	TArray<TSharedPtr<SCompoundWidget>> MainMenuList;
	// 游戏开始按钮
	MainMenuList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "StartGame", "StartGame"))
		.ItemType(EMenuItem::StartGame).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 游戏设置按钮
	MainMenuList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GameOption", "GameOption"))
		.ItemType(EMenuItem::GameOption).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 游戏退出按钮
	MainMenuList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "QuitGame", "QuitGame"))
		.ItemType(EMenuItem::QuitGame).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 界面放入map供重用
	MenuMap.Add(EMenuType::MainMenu, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "Menu", "Menu")
		, 510.f, &MainMenuList)));

	// 开始游戏界面
	TArray<TSharedPtr<SCompoundWidget>> StartGameList;
	// 新建游戏按钮
	StartGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "NewGame", "NewGame"))
		.ItemType(EMenuItem::NewGame).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 加载存档按钮
	StartGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "LoadRecord", "LoadRecord"))
		.ItemType(EMenuItem::LoadRecord).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 返回主界面按钮
	StartGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GoBack", "GoBack"))
		.ItemType(EMenuItem::StartGameGoBack).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 界面放入map供重用
	MenuMap.Add(EMenuType::StartGame, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "StartGame", "StartGame")
		, 510.f, &StartGameList)));

	// 游戏设置界面
	TArray<TSharedPtr<SCompoundWidget>> GameOptionList;
	// 实例化游戏设置的Widget
	SAssignNew(GameOptionWidget, SSDGameOptionWidget)
		.ChangeCulture(this, &SSDMenuWidget::ChangeCulture)
		.ChangeVolume(this, &SSDMenuWidget::ChangeVolume);
	// 添加控件到数组
	GameOptionList.Add(GameOptionWidget);
	GameOptionList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GoBack", "GoBack"))
		.ItemType(EMenuItem::GameOptionGoBack).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 界面放入map供重用
	MenuMap.Add(EMenuType::GameOption, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "GameOption", "GameOption")
		, 610.f, &GameOptionList)));

	// 开始新游戏界面
	TArray<TSharedPtr<SCompoundWidget>> NewGameList;
	SAssignNew(NewGameWidget, SSDNewGameWidget);
	NewGameList.Add(NewGameWidget);
	// 进入游戏按钮
	NewGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "EnterGame", "EnterGame"))
		.ItemType(EMenuItem::EnterGame).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 返回按钮
	NewGameList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GoBack", "GoBack"))
		.ItemType(EMenuItem::NewGameGoBack).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 界面放入map供重用
	MenuMap.Add(EMenuType::NewGame, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "NewGame", "NewGame")
		, 510.f, &NewGameList)));

	// 选择存档界面
	TArray<TSharedPtr<SCompoundWidget>> ChooseRecordList;
	SAssignNew(ChooseRecordWidget, SSDChooseRecordWidget);
	ChooseRecordList.Add(ChooseRecordWidget);
	// 进入存档按钮
	ChooseRecordList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "EnterRecord", "EnterRecord"))
		.ItemType(EMenuItem::EnterRecord).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 返回按钮
	ChooseRecordList.Add(SNew(SSDMenuIteamWidget).ItemText(NSLOCTEXT("SDMenu", "GoBack", "GoBack"))
		.ItemType(EMenuItem::ChooseRecordGoBack).Onclicked(this, &SSDMenuWidget::MenuIteamOnclicked));
	// 界面放入map供重用
	MenuMap.Add(EMenuType::ChooseRecord, MakeShareable(new MenuGroup(NSLOCTEXT("SDMenu", "LoadRecord", "LoadRecord")
		, 510.f, &ChooseRecordList)));

}

void SSDMenuWidget::ChooseWidget(EMenuType::Type WidgetType)
{
	// 定义是否已经显示菜单
	IsMenuShow = WidgetType != EMenuType::None;

	// 提供MenuMap选择要显示的界面
	// 移除所有组件
	ContentBox->ClearChildren();
	// 如果Menutype是None直接返回
	if (WidgetType == EMenuType::None) {
		return;
	}
	// 循环添加指定界面的组件
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It((*MenuMap.Find(WidgetType))->ChildWidget); It; ++It) {
		ContentBox->AddSlot()
			.AutoHeight()
			[(*It)->AsShared()];
	}
	// 更改标题
	TitleText->SetText((*MenuMap.Find(WidgetType))->MenuName);
	
}

// 默认高度不修改
void SSDMenuWidget::ResetWidgetSize(float NewWidget, float NewHeight = 0.f)
{
	RootSizeBox->SetWidthOverride(NewWidget);
	if (NewHeight > 0.f) {
		RootSizeBox->SetHeightOverride(NewHeight);
	}
}

void SSDMenuWidget::InitializedAnimation()
{
	// 开始延时
	const float StartDelay = 0.2f;
	// 持续时间
	const float AnimDuration = 0.4f;

	// 实例化动画播放器
	MenuAnimation = FCurveSequence();
	// 实例化曲线句柄，让其跟随动画实时变化(从0到1)
	MenuCurve = MenuAnimation.AddCurve(StartDelay, AnimDuration, ECurveEaseFunction::QuadInOut);

	// 初始设置Menu大小
	ResetWidgetSize(600.f, 510.f);

	// 选择主界面
	ChooseWidget(EMenuType::MainMenu);

	// 按钮可点击（播放动画时锁住）
	ControlLocked = false;

	// 设置动画状态为停止
	AnimState = EMenuAnim::Stop;

	// 设置动画播放器跳到结尾=1
	MenuAnimation.JumpToEnd();

}

void SSDMenuWidget::PlayClose(EMenuType::Type NewMenu)
{
	// 设置新的界面
	CurrentMenu = NewMenu;

	// 设置新高度
	CurrentHeight = (*MenuMap.Find(NewMenu))->MenuHeight;

	// 设置播放状态为Close
	AnimState = EMenuAnim::Close;

	// 播放反向动画
	MenuAnimation.PlayReverse(this->AsShared());

	// 播放切换菜单音乐
	FSlateApplication::Get().PlaySound(MenuStyle->MenuItemChangeSound);

}

void SSDMenuWidget::QuitGame()
{
	// 通过PlayerController获取
	Cast<ASDMenuController>(UGameplayStatics::GetPlayerController(GWorld, 0))->ConsoleCommand("quit");

}

void SSDMenuWidget::EnterGame()
{
	UGameplayStatics::OpenLevel(GWorld, FName("GameMap"));

	// 播放完音乐后按钮解锁
	ControlLocked = false;
}
