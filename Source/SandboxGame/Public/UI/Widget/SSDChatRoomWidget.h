// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SScrollBox;
class SEditableTextBox;
struct ChatMessItem;

DECLARE_DELEGATE_TwoParams(FPushMessage, FText, FText);

/**
 * 
 */
class SANDBOXGAME_API SSDChatRoomWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDChatRoomWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// ��Ϣ�ύ�¼�
	void SubmitEvent(const FText& SubmitText, ETextCommit::Type CommitType);

	// ��ť�¼�
	FReply SendEvent();

	// �����Ϣ
	void AddMessage(FText NewName, FText NewContent);

	// �������ײ�
	void ScrollToEnd();

public:
	FPushMessage PushMessage;

private:
	// ��ȡGameWidgetStyle
	const FSDGameStyle* GameStyle;

	// ������
	TSharedPtr<SScrollBox> ScrollBox;

	// �����ָ��
	TSharedPtr<SEditableTextBox> EditTextBox;

	// ��������
	TArray<TSharedPtr<ChatMessItem>> MessageList;

	// ��������󱣴���Ϣ����
	int MaxMessNum = 30;
};
