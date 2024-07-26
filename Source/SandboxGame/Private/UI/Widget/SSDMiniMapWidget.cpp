// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSDMiniMapWidget.h"
#include "SlateOptMacros.h"
#include "UI/Style/SDStyle.h"
#include "UI/Style/SDGameWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDMiniMapWidget::Construct(const FArguments& InArgs)
{
	// ��ȡGameStyle����ͼ��ʽ
	GameStyle = &SDStyle::Get().GetWidgetStyle<FSDGameStyle>("BPSDGameStyle");

	
	ChildSlot
	[
		SNew(SBox)
			.WidthOverride(320.f)
			.HeightOverride(320.f)
			[
				SNew(SOverlay)
					// MiniMap����ͼƬ
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SImage)
							.Image(&GameStyle->MiniMapBGBrush)
					]
					// ��ȾMiniMap��ͼƬ
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(MiniMapImage,SImage)
					]
					// ��Ⱦ�����ҰͼƬ
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
	// ��ȡ��������(ʹС��ͼ��ȾΪԲ��)
	UMaterialInterface* MiniMapMatInst = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Material/MiniMapMatInst.MiniMapMatInst'"));
	// ������̬����
	UMaterialInstanceDynamic* MiniMapMatDynamic = UMaterialInstanceDynamic::Create(MiniMapMatInst, nullptr);
	// �󶨲�������
	MiniMapMatDynamic->SetTextureParameterValue(FName("MiniMapTex"), MiniMapRender);
	// ʵ����MiniMap��ˢ(�˴��ڶ�ɽ������Դ�����ǵ�������ϷС��ͼ��Զ����...��ʵӦ��ʹ��ע��new��...)
	MiniMapBrush = new FSlateBrush();
	// ��������
	MiniMapBrush->ImageSize = FVector2D(280.f, 280.f);
	MiniMapBrush->DrawAs = ESlateBrushDrawType::Image;
	// ����Դ����
	MiniMapBrush->SetResourceObject(MiniMapMatDynamic);
	// ����ˢ��ΪMiniMapImage�ı�ˢ
	MiniMapImage->SetImage(MiniMapBrush);

	// ��Ұ�����趨
	UMaterialInterface* EnemyViewMatInst = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Material/EnemyViewMatInst.EnemyViewMatInst'"));
	// ������̬����
	EnemyViewMatDynamic = UMaterialInstanceDynamic::Create(EnemyViewMatInst, nullptr);
	// ʵ��������EnemyView��ˢ
	FSlateBrush* EnemyViewBrush = new FSlateBrush();
	// ��������
	EnemyViewBrush->ImageSize = FVector2D(280.f, 280.f);
	EnemyViewBrush->DrawAs = ESlateBrushDrawType::Image;
	// �󶨲�����Դ�ļ�
	EnemyViewBrush->SetResourceObject(EnemyViewMatDynamic);
	// ����ˢ��ΪMiniMapImage�ı�ˢ
	EnemyViewImage->SetImage(EnemyViewBrush);
	// ����ˢ����Ϊ͸����ɫ
	EnemyViewImage->SetColorAndOpacity(FLinearColor(0.3f, 1.f, 0.32f, 0.4f));

	MaxEnemyViewNum = 4;


}

void SSDMiniMapWidget::UpdateMapData(const FRotator PlayerRotator, const float MiniMapSize, const TArray<FVector2D>* EnemyPosList, const TArray<bool>* EnemyLockList, const TArray<float>* EnemyRotateList)
{
	// ��ȡYaw
	float YawDir = -PlayerRotator.Yaw;
	// ʹ�����Ǻ����ȼ��㱱��λ��
	NorthLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir)), FMath::Cos(FMath::DegreesToRadians(YawDir))) * 150.f + FVector2D(160.f, 160.f);
	// Ȼ������90�Ȼ�Բ��ȡ��������λ��
	EastLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 90.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 90.f))) * 150.f + FVector2D(160.f, 160.f);
	SouthLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 180.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 180.f))) * 150.f + FVector2D(160.f, 160.f);
	WestLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 270.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 270.f))) * 150.f + FVector2D(160.f, 160.f);

	// ��ȡ��ͼ�ߴ�
	MapSize = MiniMapSize;
	// ��ȡ���ű���
	float DPIRatio = 280.f / MapSize;
	
	// ���õ���λ�ü�����
	EnemyPos.Empty();
	EnemyLock.Empty();

	// ������������Ϣ
	TArray<float> EnemyViewRotate;
	TArray<FVector2D> EnemyViewPos;
	TArray<bool> EnemyViewLock;

	// ��ȡ������Ϣ
	for (int i = 0; i < (*EnemyPosList).Num(); ++i) {
		// �����������˾���
		float RealDistance = (*EnemyPosList)[i].Size();
		// �������С�ڵ�ͼ�뾶����Ⱦ
		if (RealDistance * 2 < MapSize) {
			// ��ȡ����widget��λ��
			EnemyPos.Add((*EnemyPosList)[i] * DPIRatio + FVector2D(160.f, 160.f));
			// �Ƿ��������
			EnemyLock.Add((*EnemyLockList)[i]);
		}
		// �������С�ڵ�ͼʵ�ʰ뾶+2000������Ⱦ������Ұ
		if (RealDistance * 2 < MapSize + 2000.f) {
			// ��Ļλ��
			EnemyViewPos.Add((*EnemyPosList)[i] * DPIRatio + FVector2D(160.f, 160.f));
			// �Ƿ��������
			EnemyViewLock.Add((*EnemyLockList)[i]);
			// �����ת��Ϣ����ʽ��Ϊ0-1
			float RotVal = -(*EnemyRotateList)[i];
			if (RotVal > 180.f) RotVal -= 360.f;
			if (RotVal < -180.f) RotVal += 360.f;
			// ���л���0-360
			RotVal += 180.f;
			// ���л���0-1
			RotVal /= 360.f;
			// ��ת180��
			RotVal = RotVal + 0.5f > 1.f ? RotVal - 0.5f : RotVal + 0.5f;
			// ��ӵ�����ת����
			EnemyViewRotate.Add(RotVal);
		}
	}

	// ������Ұ��Ⱦ����
	int ViewCount = 0;
	// �޸ĵ�����ҰͼƬ���ű���
	EnemyViewMatDynamic->SetScalarParameterValue(FName("Scale"), 1000.f / MapSize);
	for (int i = 0; i < EnemyViewPos.Num() && i < MaxEnemyViewNum; ++i, ++ViewCount) {
		FString PosName = FString("Position_") + FString::FromInt(i + 1);
		FString AngleName = FString("Angle_") + FString::FromInt(i + 1);

		// �������δ������Ҿ���Ⱦ��Ұ
		if (!EnemyViewLock[i]) {
			EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor((EnemyViewPos[i].X - 20.f) / 280.f, (EnemyViewPos[i].Y - 20.f) / 280.f, 0.f, 0.f));
			EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), EnemyViewRotate[i]);
		}
		// ������Ⱦ��Ұ
		else {
			EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor(0.f, 0.f, 0.f, 0.f));
			EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), 0.f);
		}
	}

	// �����µ���ҰͼƬ����Ⱦ
	for (ViewCount += 1; ViewCount <= MaxEnemyViewNum; ++ViewCount) {
		FString PosName = FString("Position_") + FString::FromInt(ViewCount);
		FString AngleName = FString("Angle_") + FString::FromInt(ViewCount);
		EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor(0.f, 0.f, 0.f, 0.f));
		EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), 0.f);
	}
}

int32 SSDMiniMapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// ���ø���ʵ��
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// ��Ⱦ���ͼ��
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId + 30,
		AllottedGeometry.ToPaintGeometry(FVector2D(155.f, 155.f), FVector2D(10.f, 10.f)),
		&GameStyle->PawnPointBrush,
		ESlateDrawEffect::None,
		FLinearColor(1.f, 1.f, 0.f, 1.f)
	);

	// ��Ⱦ�����ϱ��ĸ����������
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

	// ��һ��Ⱦ����λ�ã��㣩
	for (int i = 0; i < EnemyPos.Num(); ++i) {
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId + 30,
			AllottedGeometry.ToPaintGeometry(EnemyPos[i] - FVector2D(5.f, 5.f), FVector2D(10.f, 10.f)),
			&GameStyle->PawnPointBrush,
			ESlateDrawEffect::None,
			EnemyLock[i] ? FLinearColor(1.f, 0.f, 0.f, 1.f) : FLinearColor(0.f, 1.f, 0.f, 1.f)		// ���������һ���Ϊ��ɫ���������Ϊ��ɫ
		);
	}



	return LayerId;
}