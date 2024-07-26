// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Framework/Application/SlateApplication.h"
#include "Sound/SlateSound.h"
#include "TimerManager.h"

namespace SDHelper {

	FORCEINLINE void Debug(FString Message, float Duration = 3.0f) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Message);
		}
	}

	template<class UserClass>
	FORCEINLINE FTimerHandle PlayerSoundAndCall(UWorld* World, const FSlateSound Sound, UserClass* InUserObject
	, typename FTimerDelegate::TRawMethodDelegate<UserClass>::FMethodPtr InMethod) {
		// 开始播放音乐
		FSlateApplication::Get().PlaySound(Sound);

		// 创建定时器句柄
		FTimerHandle Result;

		// 获取音乐持续时间
		const float SoundDuration = FMath::Max(FSlateApplication::Get().GetSoundDuration(Sound), 0.1f);
		
		// 创建定时器委托代理并将其绑定调用函数为InUserObject::InMethod()
		FTimerDelegate Callback;
		Callback.BindRaw(InUserObject, InMethod);

		// 检查当前世界是否可用,如果可用设置开启定时委托
		if (World != nullptr) {
			World->GetTimerManager().SetTimer(Result, Callback, SoundDuration, false);
		}
		
		return Result;
	}
}