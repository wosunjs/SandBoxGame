// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template<class T>
class SANDBOXGAME_API SDSingleton
{
public:
	static TSharedPtr<T> Get();

private:
	static void Initialize();

	static TSharedRef<T> Create();

private:
	static TSharedPtr<T> TInstance;

};

template<class T>
TSharedPtr<T> SDSingleton<T>::TInstance = nullptr;

template<class T>
TSharedPtr<T> SDSingleton<T>::Get()
{
	Initialize();
	return TInstance;
}

template<class T>
void SDSingleton<T>::Initialize()
{
	if (!TInstance.IsValid()) {
		TInstance = SDSingleton<T>::Create();
	}
}

template<class T>
TSharedRef<T> SDSingleton<T>::Create()
{
	TSharedRef<T> TRef = MakeShareable(new T());
	return TRef;
}
