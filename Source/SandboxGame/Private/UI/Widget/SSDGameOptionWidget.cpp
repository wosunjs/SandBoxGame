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
	// ͨ��SDStyle��ȡSDMenuWidgetStyle�ڱ༭���е�MenuStyle
	MenuStyle = &SDStyle::Get().GetWidgetStyle<FSDMenuStyle>("BPSDMenuStyle");

	// �ڴ����ÿؼ�ʱ����Ӧί���¼�
	ChangeCulture = InArgs._ChangeCulture;
	ChangeVolume = InArgs._ChangeVolume;
	
	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(500.f)
			.HeightOverride(300.f)
			[
				SNew(SOverlay)
					// ����ͼƬ
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SImage)
							.Image(&MenuStyle->GameOptionBGBrush)
					]
					// �ؼ������SVerticalBox
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(50.f))
					[
						SNew(SVerticalBox)
							// ��һ��������Ӣ���л�
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.FillHeight(1.f)
							[
								SNew(SHorizontalBox)
									// ��һ��ˮƽ����,��������Ӣ�Ĺ�ѡ����������
									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									.FillWidth(1.f)
									[
										// ���Ĺ�ѡ��
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
										// ���Ĺ�ѡ��
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

							// �ڶ�����������
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.FillHeight(1.f)
							[
								SNew(SOverlay)
									// ���������
									+ SOverlay::Slot()
									.HAlign(HAlign_Left)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
											.Font(MenuStyle->Font_40)
											.ColorAndOpacity(MenuStyle->FontColor_Black)
											.Text(NSLOCTEXT("SDMenu", "Music", "Music"))
									]
									// �����϶���
									+ SOverlay::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									[
										SNew(SBox)
											.WidthOverride(240.f)
											[
												SNew(SOverlay)
													// ����������ͼƬ
													+ SOverlay::Slot()
													.HAlign(HAlign_Fill)
													.VAlign(VAlign_Center)
													.Padding(FMargin(30.f,0.f))
													[
														SNew(SImage)
															.Image(&MenuStyle->SliderBarBrush)
													]
													
													// ������
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
									// ������С�ٷֱ�
									+ SOverlay::Slot()
									.HAlign(HAlign_Right)
									.VAlign(VAlign_Center)
									[
										SAssignNew(MuTextBlock, STextBlock)
											.Font(MenuStyle->Font_40)
											.ColorAndOpacity(MenuStyle->FontColor_Black)
									]
							]

							// ����������
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.FillHeight(1.f)
							[
								SNew(SOverlay)
									// ���������
									+ SOverlay::Slot()
									.HAlign(HAlign_Left)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
											.Font(MenuStyle->Font_40)
											.ColorAndOpacity(MenuStyle->FontColor_Black)
											.Text(NSLOCTEXT("SDMenu", "Sound", "Sound"))
									]
									// ��Ч�϶���
									+ SOverlay::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									[
										SNew(SBox)
											.WidthOverride(240.f)
											[
												SNew(SOverlay)
													// ����������ͼƬ
													+ SOverlay::Slot()
													.HAlign(HAlign_Fill)
													.VAlign(VAlign_Center)
													.Padding(FMargin(30.f, 0.f))
													[
														SNew(SImage)
															.Image(&MenuStyle->SliderBarBrush)
													]

													// ������
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
									// ��Ч��С�ٷֱ�
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
	// ����ZhCheckBox��ʽ
	ZhcheckBox->SetUncheckedImage(&MenuStyle->UnCheckedBoxBrush);
	ZhcheckBox->SetUncheckedHoveredImage(&MenuStyle->UnCheckedBoxBrush);	// δѡ��ʱ�����ͣʱͼƬ
	ZhcheckBox->SetUncheckedPressedImage(&MenuStyle->UnCheckedBoxBrush);	// δѡ��ʱ��갴��ʱͼƬ
	ZhcheckBox->SetCheckedImage(&MenuStyle->CheckedBoxBrush);				// ѡ��ʱͼƬ
	ZhcheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBoxBrush);		// ѡ��ʱ��ͣͼƬ
	ZhcheckBox->SetCheckedPressedImage(&MenuStyle->CheckedBoxBrush);		// ѡ��ʱ����ͼƬ

	// ����EnCheckBox��ʽ
	EncheckBox->SetUncheckedImage(&MenuStyle->UnCheckedBoxBrush);
	EncheckBox->SetUncheckedHoveredImage(&MenuStyle->UnCheckedBoxBrush);	// δѡ��ʱ�����ͣʱͼƬ
	EncheckBox->SetUncheckedPressedImage(&MenuStyle->UnCheckedBoxBrush);	// δѡ��ʱ��갴��ʱͼƬ
	EncheckBox->SetCheckedImage(&MenuStyle->CheckedBoxBrush);				// ѡ��ʱͼƬ
	EncheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBoxBrush);		// ѡ��ʱ��ͣͼƬ
	EncheckBox->SetCheckedPressedImage(&MenuStyle->CheckedBoxBrush);		// ѡ��ʱ����ͼƬ

	// ���ù�ѡ״̬
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

	// ���������ٷֱ�
	float MusicValue = SDDataHandle::Get()->MusicVolume;
	MuSlider->SetValue(MusicValue);
	MuTextBlock->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(MusicValue * 100)) + FString("%")));

	// ������Ч�ٷֱ�
	float SoundValue = SDDataHandle::Get()->SoundVolume;
	SoSlider->SetValue(SoundValue);
	SoTextBlock->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(SoundValue * 100)) + FString("%")));
}

void SSDGameOptionWidget::ZhCheckBoxStateChanged(ECheckBoxState NewState)
{
	// ����ѡ�п���ʽ
	ZhcheckBox->SetIsChecked(ECheckBoxState::Checked);
	EncheckBox->SetIsChecked(ECheckBoxState::Unchecked);
	// ��֪���ݿ�����
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
	// ��ʾ�ٷֱ�
	MuTextBlock->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(value * 100)) + FString("%")));
	// ��֪���ݿ�����
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
