// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDChatRoomWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Widgets/Input/SEditableTextBox.h"


struct ChatMessItem 
{
	// 水平组件
	TSharedPtr<SHorizontalBox> CSBox;
	// 名字
	TSharedPtr<STextBlock> CSName;
	// 内容框
	TSharedPtr<SBorder> CSBorder;
	// 内容
	TSharedPtr<SMultiLineEditableText> CSContent;
	// 构造
	ChatMessItem(const FSlateBrush* EmptyBrush, const FSlateFontInfo FontInfo)
	{
		// 实例化组件
		SAssignNew(CSBox, SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				// 保存名字
				SAssignNew(CSName, STextBlock)
					.Font(FontInfo)
					.ColorAndOpacity(TAttribute<FSlateColor>(FSlateColor(FLinearColor(0.f, 1.f, 0.f, 1.f))))
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.f)
			[
				// 保存消息
				SAssignNew(CSBorder, SBorder)
					.BorderImage(EmptyBrush)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// 多行显示内容
						SAssignNew(CSContent, SMultiLineEditableText)
							.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)	// 动态显示
							.AutoWrapText(TAttribute<bool>(true))	// 自动换行
							.Font(FontInfo)

					]
			];
	}

	// 激活组件函数
	TSharedPtr<SHorizontalBox> ActiveItem(FText NewName, FText NewContent) {
		CSName->SetText(NewName);
		CSContent->SetText(NewContent);
		return CSBox;
	}

};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDChatRoomWidget::Construct(const FArguments& InArgs)
{
	// 获取GameStyle的蓝图样式
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(600.f)
			.HeightOverride(1080.f)
			[
				// 聊天室背景图片
				SNew(SBorder)
					.BorderImage(&GameStyle->ChatRoomBGBrush)
			]

			[
				SNew(SOverlay)
					// 滑动框
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(0.f, 0.f, 0.f, 80.f))
					[
						SAssignNew(ScrollBox, SScrollBox)
					]

					// 输入框及提交按钮
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.f, 1000.f, 0.f, 0.f))
					[
						SNew(SOverlay)
							// 输入框在左侧
							+SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.Padding(FMargin(0.f, 0.f, 120.f, 0.f))
							[
								SAssignNew(EditTextBox, SEditableTextBox)
									.Font(GameStyle->Font_30)
									.OnTextCommitted(this, &SSDChatRoomWidget::SubmitEvent)		// 通过EditTextBox自带的委托绑定消息提交委托
							]
							// 提交按钮在右侧
							+SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.Padding(FMargin(480.f, 0.f, 0.f, 0.f))
							[
								SNew(SButton)
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									.OnClicked(this, &SSDChatRoomWidget::SendEvent)
									[
										SNew(STextBlock)
											.Font(GameStyle->Font_30)
											.Text(NSLOCTEXT("SDGame", "Send", "Send"))
									]
							]


					]

					
			]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDChatRoomWidget::SubmitEvent(const FText& SubmitText, ETextCommit::Type CommitType)
{
	// 鼠标事件
	// 获取输入框内容
	FString MessageStr = EditTextBox->GetText().ToString();
	if (MessageStr.IsEmpty()) return;
	MessageStr = FString(": ") + MessageStr;
	AddMessage(NSLOCTEXT("SDGame", "Player", "Player"), FText::FromString(MessageStr));
	// 成功提交后重置消息框
	EditTextBox->SetText(FText::FromString(""));
	// 调用提交显示信息委托
	PushMessage.ExecuteIfBound(NSLOCTEXT("SDGame", "Player", "Player"), FText::FromString(MessageStr));
}

FReply SSDChatRoomWidget::SendEvent()
{
	// 获取输入框内容
	FString MessageStr = EditTextBox->GetText().ToString();
	if (MessageStr.IsEmpty()) return FReply::Handled();
	MessageStr = FString(": ") + MessageStr;
	AddMessage(NSLOCTEXT("SDGame", "Player", "Player"), FText::FromString(MessageStr));
	// 成功提交后重置消息框
	EditTextBox->SetText(FText::FromString(""));
	// 调用提交显示信息委托
	PushMessage.ExecuteIfBound(NSLOCTEXT("SDGame", "Player", "Player"), FText::FromString(MessageStr));
	return FReply::Handled();
}

void SSDChatRoomWidget::AddMessage(FText NewName, FText NewContent)
{
	TSharedPtr<ChatMessItem> InsertItem;

	if (MessageList.Num() < 30) {
		// 新建一条消息控件
		InsertItem = MakeShareable(new ChatMessItem(&GameStyle->EmptyBrush, GameStyle->Font_30));
		MessageList.Add(InsertItem);
		// 在UI中显示该条信息
		ScrollBox->AddSlot()
			[
				InsertItem->ActiveItem(NewName, NewContent)->AsShared()
			];
	}
	else{
		// 从序列中提取最早的一条消息
		InsertItem = MessageList[0];
		// 出队列并移除UI
		MessageList.Remove(InsertItem);
		ScrollBox->RemoveSlot(InsertItem->CSBox->AsShared());
		// 重新赋值后加入序列
		ScrollBox->AddSlot()
			[
				InsertItem->ActiveItem(NewName, NewContent)->AsShared()
			];
		MessageList.Push(InsertItem);
	}

	// 添加新消息后滑动到最底部
	ScrollBox->ScrollToEnd();
}

void SSDChatRoomWidget::ScrollToEnd()
{
	// 打开聊天室时调用
	ScrollBox->ScrollToEnd();
}
