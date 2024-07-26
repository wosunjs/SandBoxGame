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

	// 添加笔刷
	/*
	* 快捷栏容器Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush NormalContainerBrush;

	/*
	* 快捷栏被选中时Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush ChoosedContainerBrush;

	/*
	* 空Brush
	*/
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush EmptyBrush;

	// 背包的背景框笔刷
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush PackageBGBrush;

	// 合成表箭头笔刷
	UPROPERTY(EditAnywhere, Category = "Package")
	FSlateBrush CompoundArrowBrush;

	// 物品笔刷
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

	// 射线检测信息面板背景笔刷
	UPROPERTY(EditAnywhere, Category = "Info")
	FSlateBrush RayInfoBrush;

	// 准星笔刷
	UPROPERTY(EditAnywhere, Category = "Info")
	FSlateBrush PointerBrush;

	// 玩家属性背景图笔刷
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush PlayerStateBGBrush;

	// 玩家头像背景图笔刷
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush PlayerHeadBGBrush;

	// 血条笔刷
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush HPBrush;

	// 饥饿值笔刷
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush HungerBrush;

	// 玩家头像笔刷
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	FSlateBrush PlayerHeadBrush;

	// 小地图笔刷
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	FSlateBrush MiniMapBGBrush;

	// 主角小地图笔刷
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	FSlateBrush PawnPointBrush;

	// 聊天室背景图片
	UPROPERTY(EditAnywhere, Category = "ChatRoom")
	FSlateBrush ChatRoomBGBrush;

	// 添加样式
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
	* 50号字体有轮廓
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_Outline_50;

	/*
	* 40号字体
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_40;

	/*
	* 40号字体有轮廓
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_Outline_40;

	/*
	* 30号字体
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_30;

	/*
	* 20号字体
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_20;

	/*
	* 20号字体有轮廓
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_Outline_20;

	/*
	* 16号字体有轮廓
	*/
	UPROPERTY(EditAnywhere, Category = "Common")
	FSlateFontInfo Font_Outline_16;

	/*
	* 16号字体
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
