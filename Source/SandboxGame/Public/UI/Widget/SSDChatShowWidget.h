// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct ChatShowItem;

/**
 * 
 */
class SANDBOXGAME_API SSDChatShowWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDChatShowWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	// �����Ϣ
	void AddMessage(FText NewName, FText NewContent);

private:
	// ��ʼ��
	void InitlizeItem();

private:
	// ��ȡGameWidgetStyle
	const FSDGameStyle* GameStyle;

	TSharedPtr<SVerticalBox> ChatBox;

	// �Ѽ������Ϣ����
	TArray<TSharedPtr<ChatShowItem>> ActiveList;
	// δ�������Ϣ����
	TArray<TSharedPtr<ChatShowItem>> UnActiveList;

};
