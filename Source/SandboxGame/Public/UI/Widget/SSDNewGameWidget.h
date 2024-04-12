// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SOverlay.h"

class SEditableTextBox;

/**
 * 
 */
class SANDBOXGAME_API SSDNewGameWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDNewGameWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// �Ƿ���Խ�����Ϸ
	bool AllowEnterGame();

private:
	// Menu��ʽָ��
	const struct FSDMenuStyle* MenuStyle;

	// ������ı�ָ��
	TSharedPtr<SEditableTextBox> EditTextBox;

};
