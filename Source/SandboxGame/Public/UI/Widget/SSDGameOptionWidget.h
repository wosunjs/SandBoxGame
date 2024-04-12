// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/SDTypes.h"

class SSlider;

// �޸���Ӣ��ί��
DECLARE_DELEGATE_OneParam(FChangeCulture, const ECultureTeam)

// �޸�����ί��
DECLARE_DELEGATE_TwoParams(FChangeVolume, const float, const float)

/**
 * 
 */
class SANDBOXGAME_API SSDGameOptionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDGameOptionWidget)
	{}

		// ���ί��ʵ���¼�(�������󶨸�ί��)
		SLATE_EVENT(FChangeCulture, ChangeCulture)

		SLATE_EVENT(FChangeVolume, ChangeVolume)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	// ͳһ������ʽ
	void StyleInitialized();

	// ����CheckBox�¼�
	void ZhCheckBoxStateChanged(ECheckBoxState NewState);

	// Ӣ��CheckBox�¼�
	void EnCheckBoxStateChanged(ECheckBoxState NewState);

	// �����¼�
	void MusicSliderChanged(float value);

	// ��Ч�¼�
	void SoundSliderChanged(float value);

private:
	// Menu��ʽָ��
	const struct FSDMenuStyle* MenuStyle;

	// ��ȡCheckBoxָ��
	TSharedPtr<SCheckBox> EncheckBox;
	TSharedPtr<SCheckBox> ZhcheckBox;

	// ������ָ��
	TSharedPtr<SSlider> MuSlider;	// ��������
	TSharedPtr<SSlider> SoSlider;	// ��Ч

	// �������ٷֱ�ָ��
	TSharedPtr<STextBlock> MuTextBlock;	// ��������
	TSharedPtr<STextBlock> SoTextBlock;	// ��Ч

	// ί�б���
	FChangeCulture ChangeCulture;
	FChangeVolume ChangeVolume;
};
