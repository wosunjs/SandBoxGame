// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Package/SSDContainerBaseWidget.h"

/**
 * 
 */
class SANDBOXGAME_API SSDContainerInputWidget : public SSDContainerBaseWidget
{
public:
	SLATE_BEGIN_ARGS(SSDContainerInputWidget)
	{}
	
	SLATE_ATTRIBUTE(int, WorkIndex)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// 重写容器重置方法
	virtual void ResetContainerPara(int ObjectID, int Num) override;
};
