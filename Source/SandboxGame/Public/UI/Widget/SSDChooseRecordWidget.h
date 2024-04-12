// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class STextComboBox;

/**
 * 
 */
class SANDBOXGAME_API SSDChooseRecordWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDChooseRecordWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// 更新存档名
	void UpdateRecordName();

private:
	// Menu样式指针
	const struct FSDMenuStyle* MenuStyle;

	// 下拉菜单指针
	TSharedPtr<STextComboBox> RecordComboBox;
	// 存档列表字符串指针数组
	TArray<TSharedPtr<FString>> OptionsSource;

};
