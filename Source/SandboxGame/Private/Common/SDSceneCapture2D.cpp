// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SDSceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

ASDSceneCapture2D::ASDSceneCapture2D()
{
	// 设置每帧更新
	GetCaptureComponent2D()->bCaptureEveryFrame = true;

	// 设置渲染图片的格式是xxx
	GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;

	// 设置视野模式为正交模式
	GetCaptureComponent2D()->ProjectionType = ECameraProjectionMode::Orthographic;

	// 设置初始正交视野范围为3000
	GetCaptureComponent2D()->OrthoWidth = 3000.f;

	// 设置相机旋转(向下)
	SetActorRotation(FRotator(-90.f, 0.f, 0.f));
}

UTextureRenderTarget2D* ASDSceneCapture2D::GetMiniMapTex()
{
	// 创建渲染贴图
	MiniMapTex = NewObject<UTextureRenderTarget2D>();

	// 设置长宽为4的倍数
	MiniMapTex->InitAutoFormat(256, 256);

	// 绑定到渲染摄像机
	GetCaptureComponent2D()->TextureTarget = MiniMapTex;

	// 返回资源
	return MiniMapTex;
}

void ASDSceneCapture2D::UpdateTransform(FVector NormLocation, FRotator NormRotator)
{
	// 更新位置为玩家上方1000的位置
	SetActorLocation(NormLocation + FVector(0.f, 0.f, 1000.f));

	// 更新旋转为玩家的旋转
	SetActorRotation(FRotator(-90.f, NormRotator.Yaw, NormRotator.Roll));
}

void ASDSceneCapture2D::UpdateMiniMapWidth(int Delta)
{
	// 获取当前的范围
	const float PreWidth = GetCaptureComponent2D()->OrthoWidth;
	// 修改地图范围
	GetCaptureComponent2D()->OrthoWidth = FMath::Clamp<float>(PreWidth + Delta, 2000.f, 4000.f);
}

float ASDSceneCapture2D::GetMapSize()
{
	return GetCaptureComponent2D()->OrthoWidth;
}
