// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDChatShowWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Styling/SlateColor.h"

struct ChatShowItem {
	// ͸��ֵ
	float Alpha;
	// ˮƽ���
	TSharedPtr<SHorizontalBox> CSBox;
	// ����
	TSharedPtr<STextBlock> CSName;
	// ���ݿ�
	TSharedPtr<SBorder> CSBorder;
	// ����
	TSharedPtr<SMultiLineEditableText> CSContent;
	// ���캯��
	ChatShowItem(const FSlateBrush* EmptyBrush, const FSlateFontInfo FontInfo) {
		Alpha = 0.f;
		// ʵ�������
		SAssignNew(CSBox, SHorizontalBox)
			// ��Ϣ�����ʾ����
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SAssignNew(CSName, STextBlock)
					.Font(FontInfo)
					.ColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, 1.f)))
			]
			// ��Ϣ�Ҳ���ʾһ����Ϣ(��ҷ���)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.f)
			[
				SAssignNew(CSBorder, SBorder)
					.BorderImage(EmptyBrush)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SAssignNew(CSContent, SMultiLineEditableText)
							.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
							.AutoWrapText(true)
							.Font(FontInfo)
					]
			];
	}

	// �������,��������ʾ
	TSharedPtr<SHorizontalBox> ActiveItem(FText NewName, FText NewContent) {
		CSName->SetText(NewName);
		CSContent->SetText(NewContent);
		Alpha = 1.f;
		CSName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, Alpha)));
		CSBorder->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, Alpha));
		return CSBox;
	}

	// ����ʧ
	bool DeltaDisappear(float DeltaTime) {
		Alpha = FMath::Clamp<float>(Alpha - DeltaTime * 0.05f, 0.f, 1.f);	// ��ǰ����20s����Ϣ��ʧ��ʾ
		// ͨ��Alpha��������͸����
		CSName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, Alpha)));
		CSBorder->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, Alpha));
		if (Alpha == 0.f) {
			return true;	
		}
		return false;
	}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDChatShowWidget::Construct(const FArguments& InArgs)
{
	// ��ȡGameStyle����ͼ��ʽ
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");
	
	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(500.f)
			.HeightOverride(600.f)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ChatBox, SVerticalBox)
			]
	];
	
	InitlizeItem();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDChatShowWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// ��ʱ����(���浱ǰ֡ʧ�����Ϣ)
	TArray<TSharedPtr<ChatShowItem>> TempList;
	// ѭ�������Ѿ��������Ϣ
	for (TArray<TSharedPtr<ChatShowItem>>::TIterator It(ActiveList); It; ++It) {
		// ����Ѿ���ȫ������
		if ((*It)->DeltaDisappear(InDeltaTime)) {
			// ���б���ɾ��������Ϣ
			ChatBox->RemoveSlot((*It)->CSBox->AsShared());
			// �������Ϣ��ӵ���ʱ����
			TempList.Push(*It);
		}
	}
	// ����ʱ�����й��ڵ���Ϣ�Ӽ����б�����δ�����б�
	for (int i = 0; i < TempList.Num(); ++i) {
		ActiveList.Remove(TempList[i]);
		UnActiveList.Push(TempList[i]);
	}
}

void SSDChatShowWidget::AddMessage(FText NewName, FText NewContent)
{
	TSharedPtr<ChatShowItem> InsertItem;
	// ���δ�����б�Ϊ�գ��п�����Ϣ�п�����ʾ��ǰ��Ϣ��
	if (UnActiveList.Num() > 0) {
		// ��δ�����б�����ȡһ����Ϣ
		InsertItem = UnActiveList[0];
		UnActiveList.RemoveAt(0);
	}
	// ��û�п�����Ϣ��ʱ��ȡ����һ�������ú�������
	else {
		// �������б����ǰ��һ����ȡ����
		InsertItem = ActiveList[0];
		ActiveList.RemoveAt(0);
		// �Ƴ�UI
		ChatBox->RemoveSlot(InsertItem->CSBox->AsShared());
	}
	// �������Ϣ�����ӵ�UI
	ChatBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.f)
		[
			InsertItem->ActiveItem(NewName, NewContent)->AsShared()
		];
	// ����Ϣ���뼤������
	ActiveList.Push(InsertItem);
}

void SSDChatShowWidget::InitlizeItem()
{
	// ѭ��ʵ��������Ϣ
	for (int i = 0; i < 10; ++i) {
		UnActiveList.Add(MakeShareable(new ChatShowItem(&GameStyle->EmptyBrush, GameStyle->Font_16)));
	}
}