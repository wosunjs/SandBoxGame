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

	// 消息提交事件
	void SubmitEvent(const FText& SubmitText, ETextCommit::Type CommitType);

	// 按钮事件
	FReply SendEvent();

	// 添加信息
	void AddMessage(FText NewName, FText NewContent);

	// 滑动到底部
	void ScrollToEnd();

public:
	FPushMessage PushMessage;

private:
	// 获取GameWidgetStyle
	const FSDGameStyle* GameStyle;

	// 滚动框
	TSharedPtr<SScrollBox> ScrollBox;

	// 输入框指针
	TSharedPtr<SEditableTextBox> EditTextBox;

	// 保存数组
	TArray<TSharedPtr<ChatMessItem>> MessageList;

	// 聊天室最大保存消息条数
	int MaxMessNum = 30;
};
