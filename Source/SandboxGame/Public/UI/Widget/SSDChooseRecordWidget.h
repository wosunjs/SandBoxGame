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

	// ���´浵��
	void UpdateRecordName();

private:
	// Menu��ʽָ��
	const struct FSDMenuStyle* MenuStyle;

	// �����˵�ָ��
	TSharedPtr<STextComboBox> RecordComboBox;
	// �浵�б��ַ���ָ������
	TArray<TSharedPtr<FString>> OptionsSource;

};
