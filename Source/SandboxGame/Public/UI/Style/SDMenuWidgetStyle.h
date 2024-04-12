// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "SDMenuWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct SANDBOXGAME_API FSDMenuStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FSDMenuStyle();
	virtual ~FSDMenuStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FSDMenuStyle& GetDefault();
	
	/*
	* ��ʼ���汳��ͼƬBrush
	*/
	UPROPERTY(EditAnywhere, Category = "MenuHUD")
	FSlateBrush MenuHUDBackgroundBrush;

	/*
	* ��ʼ����˵�����ͼƬBrush
	*/
	UPROPERTY(EditAnywhere, Category = "Menu")
	FSlateBrush MenuBackgroundBrush;

	/*
	* �˵����ͼ��Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Menu")
	FSlateBrush LeftIconBrush;

	/*
	* �˵��Ҳ�ͼ��Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Menu")
	FSlateBrush RightIconBrush;

	/*
	* �˵�����Border��Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Menu")
	FSlateBrush TitleBorderBrush;

	/*
	* ��ťʵ����Brush
	*/
	UPROPERTY(EditAnywhere, Category = "MenuItem")
	FSlateBrush MenuItemBrush;

	/*
	* GameSet����
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSlateBrush GameOptionBGBrush;

	/*
	* CheckBox��ѡ�е�Brush
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSlateBrush CheckedBoxBrush;

	/*
	* CheckBoxδ��ѡ�е�Brush
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSlateBrush UnCheckedBoxBrush;

	/*
	* �����϶���������ˢ
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSlateBrush SliderBarBrush;

	/*
	* �����϶�����ʽ
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSliderStyle SliderStyle;

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
	* 50������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_50;

	/*
	* 40������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_40;

	/*
	* 30������
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_30;

	/*
	* ��ʼ��Ϸ����
	*/
	UPROPERTY(EditAnywhere, Category = "Sound")
	FSlateSound StartGameSound;

	/*
	* ������Ϸ����
	*/
	UPROPERTY(EditAnywhere, Category = "Sound")
	FSlateSound ExitGameSound;

	/*
	* ת����ť����
	*/
	UPROPERTY(EditAnywhere, Category = "Sound")
	FSlateSound MenuItemChangeSound;

	/*
	* Menu��������
	*/
	UPROPERTY(EditAnywhere, Category = "Sound")
	FSlateSound MenuBackgroundMusic;

};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USDMenuWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSDMenuStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
