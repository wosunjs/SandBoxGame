// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDChatShowWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"
#include "Styling/SlateColor.h"

struct ChatShowItem {
	// 透明值
	float Alpha;
	// 水平组件
	TSharedPtr<SHorizontalBox> CSBox;
	// 名字
	TSharedPtr<STextBlock> CSName;
	// 内容框
	TSharedPtr<SBorder> CSBorder;
	// 内容
	TSharedPtr<SMultiLineEditableText> CSContent;
	// 构造函数
	ChatShowItem(const FSlateBrush* EmptyBrush, const FSlateFontInfo FontInfo) {
		Alpha = 0.f;
		// 实例化组件
		SAssignNew(CSBox, SHorizontalBox)
			// 消息左侧显示名字
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SAssignNew(CSName, STextBlock)
					.Font(FontInfo)
					.ColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, 1.f)))
			]
			// 消息右侧显示一段信息(玩家发言)
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

	// 激活组件,让文字显示
	TSharedPtr<SHorizontalBox> ActiveItem(FText NewName, FText NewContent) {
		CSName->SetText(NewName);
		CSContent->SetText(NewContent);
		Alpha = 1.f;
		CSName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, Alpha)));
		CSBorder->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, Alpha));
		return CSBox;
	}

	// 逐渐消失
	bool DeltaDisappear(float DeltaTime) {
		Alpha = FMath::Clamp<float>(Alpha - DeltaTime * 0.05f, 0.f, 1.f);	// 当前设置20s后消息消失显示
		// 通过Alpha设置文字透明度
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
	// 获取GameStyle的蓝图样式
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
	// 临时序列(保存当前帧失活的信息)
	TArray<TSharedPtr<ChatShowItem>> TempList;
	// 循环遍历已经激活的信息
	for (TArray<TSharedPtr<ChatShowItem>>::TIterator It(ActiveList); It; ++It) {
		// 如果已经完全隐藏了
		if ((*It)->DeltaDisappear(InDeltaTime)) {
			// 从列表中删除这条信息
			ChatBox->RemoveSlot((*It)->CSBox->AsShared());
			// 将这个信息添加到临时序列
			TempList.Push(*It);
		}
	}
	// 将临时序列中过期的消息从激活列表移至未激活列表
	for (int i = 0; i < TempList.Num(); ++i) {
		ActiveList.Remove(TempList[i]);
		UnActiveList.Push(TempList[i]);
	}
}

void SSDChatShowWidget::AddMessage(FText NewName, FText NewContent)
{
	TSharedPtr<ChatShowItem> InsertItem;
	// 如果未激活列表不为空（有空闲消息行可以显示当前消息）
	if (UnActiveList.Num() > 0) {
		// 从未激活列表中提取一个信息
		InsertItem = UnActiveList[0];
		UnActiveList.RemoveAt(0);
	}
	// 当没有空闲消息行时，取出第一个，重置后放入最后
	else {
		// 将激活列表的最前面一个提取出来
		InsertItem = ActiveList[0];
		ActiveList.RemoveAt(0);
		// 移出UI
		ChatBox->RemoveSlot(InsertItem->CSBox->AsShared());
	}
	// 将这个信息激活并添加到UI
	ChatBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.f)
		[
			InsertItem->ActiveItem(NewName, NewContent)->AsShared()
		];
	// 将信息放入激活序列
	ActiveList.Push(InsertItem);
}

void SSDChatShowWidget::InitlizeItem()
{
	// 循环实例化空信息
	for (int i = 0; i < 10; ++i) {
		UnActiveList.Add(MakeShareable(new ChatShowItem(&GameStyle->EmptyBrush, GameStyle->Font_16)));
	}
}