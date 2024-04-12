// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Style/SDMenuWidgetStyle.h"

FSDMenuStyle::FSDMenuStyle()
{
}

FSDMenuStyle::~FSDMenuStyle()
{
}

const FName FSDMenuStyle::TypeName(TEXT("FSDMenuStyle"));

const FSDMenuStyle& FSDMenuStyle::GetDefault()
{
	static FSDMenuStyle Default;
	return Default;
}

void FSDMenuStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

