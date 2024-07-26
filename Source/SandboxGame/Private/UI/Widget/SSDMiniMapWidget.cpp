// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDMiniMapWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDMiniMapWidget::Construct(const FArguments& InArgs)
{
	// 获取GameStyle的蓝图样式
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	
	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(320.f)
			.HeightOverride(320.f)
			[
				SNew(SOverlay)
					// MiniMap背景图片
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SImage)
							.Image(&GameStyle->MiniMapBGBrush)
					]
					// 渲染MiniMap的图片
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(MiniMapImage,SImage)
					]
					// 渲染玩家视野图片
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(EnemyViewImage, SImage)
					]

			]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDMiniMapWidget::RegisterMiniMap(class UTextureRenderTarget2D* MiniMapRender)
{
	// 获取材质遮罩(使小地图渲染为圆形)
	UMaterialInterface* MiniMapMatInst = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Material/MiniMapMatInst.MiniMapMatInst'"));
	// 创建动态材质
	UMaterialInstanceDynamic* MiniMapMatDynamic = UMaterialInstanceDynamic::Create(MiniMapMatInst, nullptr);
	// 绑定材质属性
	MiniMapMatDynamic->SetTextureParameterValue(FName("MiniMapTex"), MiniMapRender);
	// 实例化MiniMap笔刷(此处在堆山创建资源，因考虑到单局游戏小地图永远存在...其实应该使用注册new的...)
	MiniMapBrush = new FSlateBrush();
	// 设置属性
	MiniMapBrush->ImageSize = FVector2D(280.f, 280.f);
	MiniMapBrush->DrawAs = ESlateBrushDrawType::Image;
	// 绑定资源材质
	MiniMapBrush->SetResourceObject(MiniMapMatDynamic);
	// 将笔刷作为MiniMapImage的笔刷
	MiniMapImage->SetImage(MiniMapBrush);

	// 视野材质设定
	UMaterialInterface* EnemyViewMatInst = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Material/EnemyViewMatInst.EnemyViewMatInst'"));
	// 创建动态材质
	EnemyViewMatDynamic = UMaterialInstanceDynamic::Create(EnemyViewMatInst, nullptr);
	// 实例化创建EnemyView笔刷
	FSlateBrush* EnemyViewBrush = new FSlateBrush();
	// 设置属性
	EnemyViewBrush->ImageSize = FVector2D(280.f, 280.f);
	EnemyViewBrush->DrawAs = ESlateBrushDrawType::Image;
	// 绑定材质资源文件
	EnemyViewBrush->SetResourceObject(EnemyViewMatDynamic);
	// 将笔刷作为MiniMapImage的笔刷
	EnemyViewImage->SetImage(EnemyViewBrush);
	// 将笔刷设置为透明绿色
	EnemyViewImage->SetColorAndOpacity(FLinearColor(0.3f, 1.f, 0.32f, 0.4f));

	MaxEnemyViewNum = 4;


}

void SSDMiniMapWidget::UpdateMapData(const FRotator PlayerRotator, const float MiniMapSize, const TArray<FVector2D>* EnemyPosList, const TArray<bool>* EnemyLockList, const TArray<float>* EnemyRotateList)
{
	// 获取Yaw
	float YawDir = -PlayerRotator.Yaw;
	// 使用三角函数先计算北的位置
	NorthLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir)), FMath::Cos(FMath::DegreesToRadians(YawDir))) * 150.f + FVector2D(160.f, 160.f);
	// 然后依次90度画圆获取其他三个位置
	EastLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 90.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 90.f))) * 150.f + FVector2D(160.f, 160.f);
	SouthLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 180.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 180.f))) * 150.f + FVector2D(160.f, 160.f);
	WestLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 270.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 270.f))) * 150.f + FVector2D(160.f, 160.f);

	// 获取地图尺寸
	MapSize = MiniMapSize;
	// 获取缩放比例
	float DPIRatio = 280.f / MapSize;
	
	// 重置敌人位置及锁定
	EnemyPos.Empty();
	EnemyLock.Empty();

	// 保存敌人相关信息
	TArray<float> EnemyViewRotate;
	TArray<FVector2D> EnemyViewPos;
	TArray<bool> EnemyViewLock;

	// 获取敌人信息
	for (int i = 0; i < (*EnemyPosList).Num(); ++i) {
		// 计算玩家与敌人距离
		float RealDistance = (*EnemyPosList)[i].Size();
		// 如果长度小于地图半径就渲染
		if (RealDistance * 2 < MapSize) {
			// 获取其在widget的位置
			EnemyPos.Add((*EnemyPosList)[i] * DPIRatio + FVector2D(160.f, 160.f));
			// 是否锁定玩家
			EnemyLock.Add((*EnemyLockList)[i]);
		}
		// 如果距离小于地图实际半径+2000，就渲染敌人视野
		if (RealDistance * 2 < MapSize + 2000.f) {
			// 屏幕位置
			EnemyViewPos.Add((*EnemyPosList)[i] * DPIRatio + FVector2D(160.f, 160.f));
			// 是否锁定玩家
			EnemyViewLock.Add((*EnemyLockList)[i]);
			// 添加旋转信息，格式化为0-1
			float RotVal = -(*EnemyRotateList)[i];
			if (RotVal > 180.f) RotVal -= 360.f;
			if (RotVal < -180.f) RotVal += 360.f;
			// 序列化到0-360
			RotVal += 180.f;
			// 序列化到0-1
			RotVal /= 360.f;
			// 旋转180度
			RotVal = RotVal + 0.5f > 1.f ? RotVal - 0.5f : RotVal + 0.5f;
			// 添加敌人旋转数组
			EnemyViewRotate.Add(RotVal);
		}
	}

	// 敌人视野渲染数量
	int ViewCount = 0;
	// 修改敌人视野图片缩放比例
	EnemyViewMatDynamic->SetScalarParameterValue(FName("Scale"), 1000.f / MapSize);
	for (int i = 0; i < EnemyViewPos.Num() && i < MaxEnemyViewNum; ++i, ++ViewCount) {
		FString PosName = FString("Position_") + FString::FromInt(i + 1);
		FString AngleName = FString("Angle_") + FString::FromInt(i + 1);

		// 如果敌人未锁定玩家就渲染视野
		if (!EnemyViewLock[i]) {
			EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor((EnemyViewPos[i].X - 20.f) / 280.f, (EnemyViewPos[i].Y - 20.f) / 280.f, 0.f, 0.f));
			EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), EnemyViewRotate[i]);
		}
		// 否则不渲染视野
		else {
			EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor(0.f, 0.f, 0.f, 0.f));
			EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), 0.f);
		}
	}

	// 把余下的视野图片不渲染
	for (ViewCount += 1; ViewCount <= MaxEnemyViewNum; ++ViewCount) {
		FString PosName = FString("Position_") + FString::FromInt(ViewCount);
		FString AngleName = FString("Angle_") + FString::FromInt(ViewCount);
		EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor(0.f, 0.f, 0.f, 0.f));
		EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), 0.f);
	}
}

int32 SSDMiniMapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// 调用父类实现
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// 渲染玩家图标
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId + 30,
		AllottedGeometry.ToPaintGeometry(FVector2D(155.f, 155.f), FVector2D(10.f, 10.f)),
		&GameStyle->PawnPointBrush,
		ESlateDrawEffect::None,
		FLinearColor(1.f, 1.f, 0.f, 1.f)
	);

	// 渲染东西南北四个方向的文字
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId + 10,
		AllottedGeometry.ToPaintGeometry(NorthLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)),
		NSLOCTEXT("SDGame", "N", "N"),
		GameStyle->Font_16,
		ESlateDrawEffect::None,
		FLinearColor(1.f, 1.f, 1.f, 1.f)
	);
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId + 10,
		AllottedGeometry.ToPaintGeometry(SouthLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)),
		NSLOCTEXT("SDGame", "S", "S"),
		GameStyle->Font_16,
		ESlateDrawEffect::None,
		FLinearColor(1.f, 1.f, 1.f, 1.f)
	);
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId + 10,
		AllottedGeometry.ToPaintGeometry(EastLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)),
		NSLOCTEXT("SDGame", "E", "E"),
		GameStyle->Font_16,
		ESlateDrawEffect::None,
		FLinearColor(1.f, 1.f, 1.f, 1.f)
	);
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId + 10,
		AllottedGeometry.ToPaintGeometry(WestLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)),
		NSLOCTEXT("SDGame", "W", "W"),
		GameStyle->Font_16,
		ESlateDrawEffect::None,
		FLinearColor(1.f, 1.f, 1.f, 1.f)
	);

	// 逐一渲染敌人位置（点）
	for (int i = 0; i < EnemyPos.Num(); ++i) {
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId + 30,
			AllottedGeometry.ToPaintGeometry(EnemyPos[i] - FVector2D(5.f, 5.f), FVector2D(10.f, 10.f)),
			&GameStyle->PawnPointBrush,
			ESlateDrawEffect::None,
			EnemyLock[i] ? FLinearColor(1.f, 0.f, 0.f, 1.f) : FLinearColor(0.f, 1.f, 0.f, 1.f)		// 如果锁定玩家绘制为红色，否则绘制为绿色
		);
	}



	return LayerId;
}