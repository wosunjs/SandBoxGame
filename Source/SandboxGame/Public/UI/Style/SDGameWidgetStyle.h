// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "SDGameWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct SANDBOXGAME_API FSDGameStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FSDGameStyle();
	virtual ~FSDGameStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FSDGameStyle& GetDefault();

	// ��ӱ�ˢ
	/*
	* ���������Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush NormalContainerBrush;

	/*
	* �������ѡ��ʱBrush
	*/
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ChoosedContainerBrush;

	/*
	* ��Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush EmptyBrush;

	// �����ı������ˢ
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush PackageBGBrush;

	// �ϳɱ��ͷ��ˢ
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush CompoundArrowBrush;

	// ��Ʒ��ˢ
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ObjectBrush_1;
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ObjectBrush_2;
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ObjectBrush_3;
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ObjectBrush_4;
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ObjectBrush_5;
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ObjectBrush_6;
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ObjectBrush_7;

	// ���߼����Ϣ��屳����ˢ
	UPROPERTY(EditAnywhere, Category = "Info")
	FSlateBrush RayInfoBrush;

	// ׼�Ǳ�ˢ
	UPROPERTY(EditAnywhere, Category = "Info")
	FSlateBrush PointerBrush;

	// ������Ա���ͼ��ˢ
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush PlayerStateBGBrush;

	// ���ͷ�񱳾�ͼ��ˢ
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush PlayerHeadBGBrush;

	// Ѫ����ˢ
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush HPBrush;

	// ����ֵ��ˢ
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush HungerBrush;

	// ���ͷ���ˢ
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush PlayerHeadBrush;

	// С��ͼ��ˢ
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	FSlateBrush MiniMapBGBrush;

	// ����С��ͼ��ˢ
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	FSlateBrush PawnPointBrush;

	// �����ұ���ͼƬ
	UPROPERTY(EditAnywhere, Category = "ChatRoom")
	FSlateBrush ChatRoomBGBrush;

	// �����ʽ
	/*
	* ��ɫ��ɫ
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FLinearColor FontColor_White;

	/*
	* ��ɫ��ɫ
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FLinearColor FontColor_Black;

	/*
	* 60������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_60;

	/*
	* 50������������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_Outline_50;

	/*
	* 40������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_40;

	/*
	* 40������������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_Outline_40;

	/*
	* 30������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_30;

	/*
	* 20������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_20;

	/*
	* 20������������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_Outline_20;

	/*
	* 16������������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_Outline_16;

	/*
	* 16������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_16;


};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USDGameWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSDGameStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
