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
		// ��ʼ��������
		FSlateApplication::Get().PlaySound(Sound);

		// ������ʱ�����
		FTimerHandle Result;

		// ��ȡ���ֳ���ʱ��
		const float SoundDuration = FMath::Max(FSlateApplication::Get().GetSoundDuration(Sound), 0.1f);
		
		// ������ʱ��ί�д�������󶨵��ú���ΪInUserObject::InMethod()
		FTimerDelegate Callback;
		Callback.BindRaw(InUserObject, InMethod);

		// ��鵱ǰ�����Ƿ����,����������ÿ�����ʱί��
		if (World != nullptr) {
			World->GetTimerManager().SetTimer(Result, Callback, SoundDuration, false);
		}
		
		return Result;
	}
}