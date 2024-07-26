// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Package/SSDContainerBaseWidget.h"

/**
 * 
 */
class SANDBOXGAME_API SSDContainerOutputWidget : public SSDContainerBaseWidget
{
public:
	SLATE_BEGIN_ARGS(SSDContainerOutputWidget)
	{}

	SLATE_ATTRIBUTE(int, WorkIndex)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// ÖØÐ´×ó¼ü²Ù×÷
	virtual void LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum) override;

	// ÖØÐ´ÓÒ¼ü¼ü²Ù×÷
	virtual void RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum) override;
};
