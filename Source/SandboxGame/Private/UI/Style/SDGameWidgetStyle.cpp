// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Style/SDGameWidgetStyle.h"

FSDGameStyle::FSDGameStyle()
{
}

FSDGameStyle::~FSDGameStyle()
{
}

const FName FSDGameStyle::TypeName(TEXT("FSDGameStyle"));

const FSDGameStyle& FSDGameStyle::GetDefault()
{
	static FSDGameStyle Default;
	return Default;
}

void FSDGameStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

