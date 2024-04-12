// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/SDTypes.h"

// ����һ�����밴ť���ͣ���ΪFIteamClicked��ί��
DECLARE_DELEGATE_OneParam(FIteamClicked, const EMenuItem::Type)

/**
 * 
 */
class SANDBOXGAME_API SSDMenuIteamWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDMenuIteamWidget)
	{}

	// �ú���ؼ���ӱ����������ͣ� ���ƣ�
	SLATE_ATTRIBUTE(FText, ItemText)

	SLATE_ATTRIBUTE(EMenuItem::Type, ItemType)

	// �ú���ؼ������Ӧ�¼�����ί������ �¼��������ƣ���ÿ�ε���������Onclick�󶨵ĺ���
	SLATE_EVENT(FIteamClicked, Onclicked)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// ��д����������Ӱ������ַ���:��������¡��뿪
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

private:
	// ��ȡ��ť���º����ɫ
	FSlateColor GetTintColor() const;

private:
	// ���º󴥷����¼�ί�У�ͨ��Construct������SLATE���������ͨ��InArgs������ɳ�ʼ����
	FIteamClicked Onclicked;

	// ��ť����
	EMenuItem::Type ItemType;

	// ��ȡMenu��ʽ
	const struct FSDMenuStyle* MenuStyle;

	// ��ť�Ƿ��Ѿ�����
	bool IsMouseButtonDown;

};
