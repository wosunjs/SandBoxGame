// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDGameOptionWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDMenuWidgetStyle.h"
#include "Data/SDDataHandle.h"
#include "Data/SDTypes.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDGameOptionWidget::Construct(const FArguments& InArgs)
{
	// 通过SDStyle获取SDMenuWidgetStyle在编辑器中的MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// 在创建该控件时绑定相应委托事件
	ChangeCulture = InArgs._ChangeCulture;
	ChangeVolume = InArgs._ChangeVolume;
	
	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(500.f)
			.HeightOverride(300.f)
			[
				SNew(SOverlay)
					// 背景图片
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SImage)
							.Image(&MenuStyle->GameOptionBGBrush)
					]
					// 控件栏插槽SVerticalBox
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(50.f))
					[
						SNew(SVerticalBox)
							// 第一行设置中英文切换
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.FillHeight(1.f)
							[
								SNew(SHorizontalBox)
									// 第一行水平布局,由两个中英文勾选框和文字组成
									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									.FillWidth(1.f)
									[
										// 中文勾选框
										SAssignNew(ZhcheckBox, SCheckBox)
											.OnCheckStateChanged(this, &SSDGameOptionWidget::ZhCheckBoxStateChanged)
											[
												SNew(STextBlock)
													.Font(MenuStyle->Font_40)
													.ColorAndOpacity(MenuStyle->FontColor_Black)
													.Text(NSLOCTEXT("SDMenu", "Chinese", "Chinese"))
											]
									]

									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									.FillWidth(1.f)
									[
										// 中文勾选框
										SAssignNew(EncheckBox, SCheckBox)
											.OnCheckStateChanged(this, &SSDGameOptionWidget::EnCheckBoxStateChanged)
											[
												SNew(STextBlock)
													.Font(MenuStyle->Font_40)
													.ColorAndOpacity(MenuStyle->FontColor_Black)
													.Text(NSLOCTEXT("SDMenu", "English", "English"))
											]
									]

							]

							// 第二行设置音量
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.FillHeight(1.f)
							[
								SNew(SOverlay)
									// 左对其字体
									+ SOverlay::Slot()
									.HAlign(HAlign_Left)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
											.Font(MenuStyle->Font_40)
											.ColorAndOpacity(MenuStyle->FontColor_Black)
											.Text(NSLOCTEXT("SDMenu", "Music", "Music"))
									]
									// 音乐拖动条
									+ SOverlay::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									[
										SNew(SBox)
											.WidthOverride(240.f)
											[
												SNew(SOverlay)
													// 滑动条背景图片
													+ SOverlay::Slot()
													.HAlign(HAlign_Fill)
													.VAlign(VAlign_Center)
													.Padding(FMargin(30.f,0.f))
													[
														SNew(SImage)
															.Image(&MenuStyle->SliderBarBrush)
													]
													
													// 滑动条
													+ SOverlay::Slot()
													.HAlign(HAlign_Fill)
													.VAlign(VAlign_Center)
													[
														SAssignNew(MuSlider, SSlider)
															.Style(&MenuStyle->SliderStyle)
															.OnValueChanged(this, &SSDGameOptionWidget::MusicSliderChanged)
													]
											]

									]
									// 音量大小百分比
									+ SOverlay::Slot()
									.HAlign(HAlign_Right)
									.VAlign(VAlign_Center)
									[
										SAssignNew(MuTextBlock, STextBlock)
											.Font(MenuStyle->Font_40)
											.ColorAndOpacity(MenuStyle->FontColor_Black)
									]
							]

							// 第三行设置
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.FillHeight(1.f)
							[
								SNew(SOverlay)
									// 左对其字体
									+ SOverlay::Slot()
									.HAlign(HAlign_Left)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
											.Font(MenuStyle->Font_40)
											.ColorAndOpacity(MenuStyle->FontColor_Black)
											.Text(NSLOCTEXT("SDMenu", "Sound", "Sound"))
									]
									// 音效拖动条
									+ SOverlay::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									[
										SNew(SBox)
											.WidthOverride(240.f)
											[
												SNew(SOverlay)
													// 滑动条背景图片
													+ SOverlay::Slot()
													.HAlign(HAlign_Fill)
													.VAlign(VAlign_Center)
													.Padding(FMargin(30.f, 0.f))
													[
														SNew(SImage)
															.Image(&MenuStyle->SliderBarBrush)
													]

													// 滑动条
													+ SOverlay::Slot()
													.HAlign(HAlign_Fill)
													.VAlign(VAlign_Center)
													[
														SAssignNew(SoSlider, SSlider)
															.Style(&MenuStyle->SliderStyle)
															.OnValueChanged(this, &SSDGameOptionWidget::SoundSliderChanged)
													]
											]

									]
									// 音效大小百分比
									+ SOverlay::Slot()
									.HAlign(HAlign_Right)
									.VAlign(VAlign_Center)
									[
										SAssignNew(SoTextBlock, STextBlock)
											.Font(MenuStyle->Font_40)
											.ColorAndOpacity(MenuStyle->FontColor_Black)
									]
							]

					]
			]
	];
	
	StyleInitialized();
}

void SSDGameOptionWidget::StyleInitialized()
{
	// 设置ZhCheckBox样式
	ZhcheckBox->SetUncheckedImage(&MenuStyle->UnCheckedBoxBrush);
	ZhcheckBox->SetUncheckedHoveredImage(&MenuStyle->UnCheckedBoxBrush);	// 未选中时鼠标悬停时图片
	ZhcheckBox->SetUncheckedPressedImage(&MenuStyle->UnCheckedBoxBrush);	// 未选中时鼠标按下时图片
	ZhcheckBox->SetCheckedImage(&MenuStyle->CheckedBoxBrush);				// 选中时图片
	ZhcheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBoxBrush);		// 选中时悬停图片
	ZhcheckBox->SetCheckedPressedImage(&MenuStyle->CheckedBoxBrush);		// 选中时按下图片

	// 设置EnCheckBox样式
	EncheckBox->SetUncheckedImage(&MenuStyle->UnCheckedBoxBrush);
	EncheckBox->SetUncheckedHoveredImage(&MenuStyle->UnCheckedBoxBrush);	// 未选中时鼠标悬停时图片
	EncheckBox->SetUncheckedPressedImage(&MenuStyle->UnCheckedBoxBrush);	// 未选中时鼠标按下时图片
	EncheckBox->SetCheckedImage(&MenuStyle->CheckedBoxBrush);				// 选中时图片
	EncheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBoxBrush);		// 选中时悬停图片
	EncheckBox->SetCheckedPressedImage(&MenuStyle->CheckedBoxBrush);		// 选中时按下图片

	// 设置勾选状态
	switch (SDDataHandle::Get()->GetLocalizationCulture()) {
		case ECultureTeam::EN:
			EncheckBox->SetIsChecked(ECheckBoxState::Checked);
			ZhcheckBox->SetIsChecked(ECheckBoxState::Unchecked);
			break;
		case ECultureTeam::ZH:
			ZhcheckBox->SetIsChecked(ECheckBoxState::Checked);
			EncheckBox->SetIsChecked(ECheckBoxState::Unchecked);
			break;
	}

	// 设置音量百分比
	float MusicValue = SDDataHandle::Get()->MusicVolume;
	MuSlider->SetValue(MusicValue);
	MuTextBlock->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(MusicValue * 100)) + FString("%")));

	// 设置音效百分比
	float SoundValue = SDDataHandle::Get()->SoundVolume;
	SoSlider->SetValue(SoundValue);
	SoTextBlock->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(SoundValue * 100)) + FString("%")));
}

void SSDGameOptionWidget::ZhCheckBoxStateChanged(ECheckBoxState NewState)
{
	// 设置选中框样式
	ZhcheckBox->SetIsChecked(ECheckBoxState::Checked);
	EncheckBox->SetIsChecked(ECheckBoxState::Unchecked);
	// 告知数据控制类
	//SDDataHandle::Get()->ChangeLocalizationCulture(ECultureTeam::ZH);
	ChangeCulture.ExecuteIfBound(ECultureTeam::ZH);
}

void SSDGameOptionWidget::EnCheckBoxStateChanged(ECheckBoxState NewState)
{
	EncheckBox->SetIsChecked(ECheckBoxState::Checked);
	ZhcheckBox->SetIsChecked(ECheckBoxState::Unchecked);
	//SDDataHandle::Get()->ChangeLocalizationCulture(ECultureTeam::EN);
	ChangeCulture.ExecuteIfBound(ECultureTeam::EN);
}

void SSDGameOptionWidget::MusicSliderChanged(float value)
{
	// 显示百分比
	MuTextBlock->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(value * 100)) + FString("%")));
	// 告知数据控制类
	//SDDataHandle::Get()->ChangeVolume(value, -1.f);
	ChangeVolume.ExecuteIfBound(value, -1.f);
}

void SSDGameOptionWidget::SoundSliderChanged(float value)
{
	SoTextBlock->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(value * 100)) + FString("%")));
	//SDDataHandle::Get()->ChangeVolume(-1.f, value);
	ChangeVolume.ExecuteIfBound(-1.f, value);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
