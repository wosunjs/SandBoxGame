// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDChatRoomWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Widgets/Input/SEditableTextBox.h"


struct ChatMessItem 
{
	// ˮƽ���
	TSharedPtr<SHorizontalBox> CSBox;
	// ����
	TSharedPtr<STextBlock> CSName;
	// ���ݿ�
	TSharedPtr<SBorder> CSBorder;
	// ����
	TSharedPtr<SMultiLineEditableText> CSContent;
	// ����
	ChatMessItem(const FSlateBrush* EmptyBrush, const FSlateFontInfo FontInfo)
	{
		// ʵ�������
		SAssignNew(CSBox, SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				// ��������
				SAssignNew(CSName, STextBlock)
					.Font(FontInfo)
					.ColorAndOpacity(TAttribute<FSlateColor>(FSlateColor(FLinearColor(0.f, 1.f, 0.f, 1.f))))
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.f)
			[
				// ������Ϣ
				SAssignNew(CSBorder, SBorder)
					.BorderImage(EmptyBrush)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// ������ʾ����
						SAssignNew(CSContent, SMultiLineEditableText)
							.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)	// ��̬��ʾ
							.AutoWrapText(TAttribute<bool>(true))	// �Զ�����
							.Font(FontInfo)

					]
			];
	}

	// �����������
	TSharedPtr<SHorizontalBox> ActiveItem(FText NewName, FText NewContent) {
		CSName->SetText(NewName);
		CSContent->SetText(NewContent);
		return CSBox;
	}

};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDChatRoomWidget::Construct(const FArguments& InArgs)
{
	// ��ȡGameStyle����ͼ��ʽ
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(600.f)
			.HeightOverride(1080.f)
			[
				// �����ұ���ͼƬ
				SNew(SBorder)
					.BorderImage(&GameStyle->ChatRoomBGBrush)
			]

			[
				SNew(SOverlay)
					// ������
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(0.f, 0.f, 0.f, 80.f))
					[
						SAssignNew(ScrollBox, SScrollBox)
					]

					// ������ύ��ť
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.f, 1000.f, 0.f, 0.f))
					[
						SNew(SOverlay)
							// ����������
							+SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.Padding(FMargin(0.f, 0.f, 120.f, 0.f))
							[
								SAssignNew(EditTextBox, SEditableTextBox)
									.Font(GameStyle->Font_30)
									.OnTextCommitted(this, &SSDChatRoomWidget::SubmitEvent)		// ͨ��EditTextBox�Դ���ί�а���Ϣ�ύί��
							]
							// �ύ��ť���Ҳ�
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
	// ����¼�
	// ��ȡ���������
	FString MessageStr = EditTextBox->GetText().ToString();
	if (MessageStr.IsEmpty()) return;
	MessageStr = FString(": ") + MessageStr;
	AddMessage(NSLOCTEXT("SDGame", "Player", "Player"), FText::FromString(MessageStr));
	// �ɹ��ύ��������Ϣ��
	EditTextBox->SetText(FText::FromString(""));
	// �����ύ��ʾ��Ϣί��
	PushMessage.ExecuteIfBound(NSLOCTEXT("SDGame", "Player", "Player"), FText::FromString(MessageStr));
}

FReply SSDChatRoomWidget::SendEvent()
{
	// ��ȡ���������
	FString MessageStr = EditTextBox->GetText().ToString();
	if (MessageStr.IsEmpty()) return FReply::Handled();
	MessageStr = FString(": ") + MessageStr;
	AddMessage(NSLOCTEXT("SDGame", "Player", "Player"), FText::FromString(MessageStr));
	// �ɹ��ύ��������Ϣ��
	EditTextBox->SetText(FText::FromString(""));
	// �����ύ��ʾ��Ϣί��
	PushMessage.ExecuteIfBound(NSLOCTEXT("SDGame", "Player", "Player"), FText::FromString(MessageStr));
	return FReply::Handled();
}

void SSDChatRoomWidget::AddMessage(FText NewName, FText NewContent)
{
	TSharedPtr<ChatMessItem> InsertItem;

	if (MessageList.Num() < 30) {
		// �½�һ����Ϣ�ؼ�
		InsertItem = MakeShareable(new ChatMessItem(&GameStyle->EmptyBrush, GameStyle->Font_30));
		MessageList.Add(InsertItem);
		// ��UI����ʾ������Ϣ
		ScrollBox->AddSlot()
			[
				InsertItem->ActiveItem(NewName, NewContent)->AsShared()
			];
	}
	else{
		// ����������ȡ�����һ����Ϣ
		InsertItem = MessageList[0];
		// �����в��Ƴ�UI
		MessageList.Remove(InsertItem);
		ScrollBox->RemoveSlot(InsertItem->CSBox->AsShared());
		// ���¸�ֵ���������
		ScrollBox->AddSlot()
			[
				InsertItem->ActiveItem(NewName, NewContent)->AsShared()
			];
		MessageList.Push(InsertItem);
	}

	// �������Ϣ�󻬶�����ײ�
	ScrollBox->ScrollToEnd();
}

void SSDChatRoomWidget::ScrollToEnd()
{
	// ��������ʱ����
	ScrollBox->ScrollToEnd();
}
