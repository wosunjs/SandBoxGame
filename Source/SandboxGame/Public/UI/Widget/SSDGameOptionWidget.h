// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/SDTypes.h"

class SSlider;

// 修改中英文委托
DECLARE_DELEGATE_OneParam(FChangeCulture, const ECultureTeam)

// 修改音量委托
DECLARE_DELEGATE_TwoParams(FChangeVolume, const float, const float)

/**
 * 
 */
class SANDBOXGAME_API SSDGameOptionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDGameOptionWidget)
	{}

		// 添加委托实现事件(将函数绑定给委托)
		SLATE_EVENT(FChangeCulture, ChangeCulture)

		SLATE_EVENT(FChangeVolume, ChangeVolume)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	// 统一设置样式
	void StyleInitialized();

	// 中文CheckBox事件
	void ZhCheckBoxStateChanged(ECheckBoxState NewState);

	// 英文CheckBox事件
	void EnCheckBoxStateChanged(ECheckBoxState NewState);

	// 音量事件
	void MusicSliderChanged(float value);

	// 音效事件
	void SoundSliderChanged(float value);

private:
	// Menu样式指针
	const struct FSDMenuStyle* MenuStyle;

	// 获取CheckBox指针
	TSharedPtr<SCheckBox> EncheckBox;
	TSharedPtr<SCheckBox> ZhcheckBox;

	// 滑动条指针
	TSharedPtr<SSlider> MuSlider;	// 背景音乐
	TSharedPtr<SSlider> SoSlider;	// 音效

	// 进度条百分比指针
	TSharedPtr<STextBlock> MuTextBlock;	// 背景音乐
	TSharedPtr<STextBlock> SoTextBlock;	// 音效

	// 委托变量
	FChangeCulture ChangeCulture;
	FChangeVolume ChangeVolume;
};
