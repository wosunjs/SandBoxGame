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
	* 初始界面背景图片Brush
	*/
	UPROPERTY(EditAnywhere, Category = "MenuHUD")
	FSlateBrush MenuHUDBackgroundBrush;

	/*
	* 初始界面菜单背景图片Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Menu")
	FSlateBrush MenuBackgroundBrush;

	/*
	* 菜单左侧图标Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Menu")
	FSlateBrush LeftIconBrush;

	/*
	* 菜单右侧图标Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Menu")
	FSlateBrush RightIconBrush;

	/*
	* 菜单标题Border的Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Menu")
	FSlateBrush TitleBorderBrush;

	/*
	* 按钮实例的Brush
	*/
	UPROPERTY(EditAnywhere, Category = "MenuItem")
	FSlateBrush MenuItemBrush;

	/*
	* GameSet背景
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSlateBrush GameOptionBGBrush;

	/*
	* CheckBox被选中的Brush
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSlateBrush CheckedBoxBrush;

	/*
	* CheckBox未被选中的Brush
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSlateBrush UnCheckedBoxBrush;

	/*
	* 音乐拖动条背景笔刷
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSlateBrush SliderBarBrush;

	/*
	* 音乐拖动条样式
	*/
	UPROPERTY(EditAnywhere, Category = "GameOption")
	FSliderStyle SliderStyle;

	/*
	* 白色颜色
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FLinearColor FontColor_White;

	/*
	* 黑色颜色
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FLinearColor FontColor_Black;

	/*
	* 60号字体
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_60;

	/*
	* 50号字体
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_50;

	/*
	* 40号字体
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_40;

	/*
	* 30号字体
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_30;

	/*
	* 开始游戏声音
	*/
	UPROPERTY(EditAnywhere, Category = "Sound")
	FSlateSound StartGameSound;

	/*
	* 结束游戏声音
	*/
	UPROPERTY(EditAnywhere, Category = "Sound")
	FSlateSound ExitGameSound;

	/*
	* 转换按钮声音
	*/
	UPROPERTY(EditAnywhere, Category = "Sound")
	FSlateSound MenuItemChangeSound;

	/*
	* Menu背景声音
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
