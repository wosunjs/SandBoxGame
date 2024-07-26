// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SDSceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

ASDSceneCapture2D::ASDSceneCapture2D()
{
	// ����ÿ֡����
	GetCaptureComponent2D()->bCaptureEveryFrame = true;

	// ������ȾͼƬ�ĸ�ʽ��xxx
	GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;

	// ������ҰģʽΪ����ģʽ
	GetCaptureComponent2D()->ProjectionType = ECameraProjectionMode::Orthographic;

	// ���ó�ʼ������Ұ��ΧΪ3000
	GetCaptureComponent2D()->OrthoWidth = 3000.f;

	// ���������ת(����)
	SetActorRotation(FRotator(-90.f, 0.f, 0.f));
}

UTextureRenderTarget2D* ASDSceneCapture2D::GetMiniMapTex()
{
	// ������Ⱦ��ͼ
	MiniMapTex = NewObject<UTextureRenderTarget2D>();

	// ���ó���Ϊ4�ı���
	MiniMapTex->InitAutoFormat(256, 256);

	// �󶨵���Ⱦ�����
	GetCaptureComponent2D()->TextureTarget = MiniMapTex;

	// ������Դ
	return MiniMapTex;
}

void ASDSceneCapture2D::UpdateTransform(FVector NormLocation, FRotator NormRotator)
{
	// ����λ��Ϊ����Ϸ�1000��λ��
	SetActorLocation(NormLocation + FVector(0.f, 0.f, 1000.f));

	// ������תΪ��ҵ���ת
	SetActorRotation(FRotator(-90.f, NormRotator.Yaw, NormRotator.Roll));
}

void ASDSceneCapture2D::UpdateMiniMapWidth(int Delta)
{
	// ��ȡ��ǰ�ķ�Χ
	const float PreWidth = GetCaptureComponent2D()->OrthoWidth;
	// �޸ĵ�ͼ��Χ
	GetCaptureComponent2D()->OrthoWidth = FMath::Clamp<float>(PreWidth + Delta, 2000.f, 4000.f);
}

float ASDSceneCapture2D::GetMapSize()
{
	return GetCaptureComponent2D()->OrthoWidth;
}
