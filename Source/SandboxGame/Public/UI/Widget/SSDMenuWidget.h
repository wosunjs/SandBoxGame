// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/SDTypes.h"

class SBox;
class STextBlock;
class SVerticalBox;
struct MenuGroup;
class SSDGameOptionWidget;
class SSDNewGameWidget;
class SSDChooseRecordWidget;


/**
 * 
 */
class SANDBOXGAME_API SSDMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDMenuWidget)
	{}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// ��дtick����
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	// MenuIteam�󶨵ķ���(���ʱSSDMenuIteamWidget��ͨ����ť����ִ�е��ø��¼�)
	void MenuIteamOnclicked(EMenuItem::Type ItemType);

	// �޸�����
	void ChangeCulture(ECultureTeam Culture);

	// �޸�����
	void ChangeVolume(const float MusicVolume, const float SoundVolume);

	// ��ʼ�����еĿؼ�
	void InitializeMenuList();

	// ѡ����ʾ�Ľ���
	void ChooseWidget(EMenuType::Type WidgetType);

	// �޸Ĳ˵��Ĵ�С
	void ResetWidgetSize(float NewWidget, float NewHeight);

	// ��ʼ���������
	void InitializedAnimation();

	// ���Źرն���
	void PlayClose(EMenuType::Type NewMenu);

	// �˳���Ϸ
	void QuitGame();

private:
	// ���ڵ�ָ��
	TSharedPtr<SBox> RootSizeBox;

	// ��ȡMenu��ʽ
	const struct FSDMenuStyle* MenuStyle;

	// ��������
	TSharedPtr<STextBlock> TitleText;

	// ��ֱ�б�,������а�ť
	TSharedPtr<SVerticalBox> ContentBox;

	// ����˵���map��key���˵��ؼ����� value���ؼ�ָ��
	TMap<EMenuType::Type, TSharedPtr<MenuGroup>> MenuMap;

	// ��Ϸ����widget��ָ��
	TSharedPtr<SSDGameOptionWidget> GameOptionWidget;

	// ����Ϸ�ؼ�ָ��
	TSharedPtr<SSDNewGameWidget> NewGameWidget;

	// ѡ��浵�ؼ�ָ��
	TSharedPtr<SSDChooseRecordWidget> ChooseRecordWidget;

	// ����������
	FCurveSequence MenuAnimation;

	// ���߿�����
	FCurveHandle MenuCurve;

	// ���������µĳ���
	float CurrentHeight;

	// �Ƿ��Ѿ���ʾMenu���
	bool IsMenuShow;

	// �Ƿ���ס��ť
	bool ControlLocked;

	// ���浱ǰ�Ķ���״̬
	EMenuAnim::Type AnimState;

	// ���浱ǰ�Ĳ˵�
	EMenuType::Type CurrentMenu;

};
