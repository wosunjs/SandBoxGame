// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SDPackageManager.h"
#include "Data/SDDataHandle.h"
#include "common/SDHelper.h"

// 单例指针初始化赋值
TSharedPtr<SDPackageManager> SDPackageManager::PackageInstance = nullptr;

SDPackageManager::SDPackageManager()
{
	// 初始化物品ID和数量为0
	ObjectIndex = 0;
	ObjectNum = 0;
}

void SDPackageManager::Initialize()
{
	if (PackageInstance == nullptr) {
		PackageInstance = Create();
	}
}

TSharedPtr<SDPackageManager> SDPackageManager::Get()
{
	Initialize();
	return PackageInstance;
}

void SDPackageManager::InsertContainer(TSharedPtr<SSDContainerBaseWidget> Container, EContainerType::Type InsertType)
{
	switch (InsertType)
	{
	case EContainerType::Output:
		OutputContainer = Container;
		// 绑定合成输出委托和丢弃委托
		OutputContainer->CompoundOutput.BindRaw(this, &SDPackageManager::CompoundOutput);
		OutputContainer->ThrowObject.BindRaw(this, &SDPackageManager::ThrowObject);
		break;
	case EContainerType::Input:
		// 绑定合成输入委托
		Container->CompoundInput.BindRaw(this, &SDPackageManager::CompoundInput);
		InputContainerList.Add(Container);
		break;
	case EContainerType::Normal:
		NormalContainerList.Add(Container);
		break;
	case EContainerType::Shortcut:
		// 绑定快捷栏修改委托
		Container->PackageShortChange.BindRaw(this, &SDPackageManager::PackageShortChange);
		ShortcutContainerList.Add(Container);
		break;
	default:
		break;
	}
}

TSharedRef<SDPackageManager> SDPackageManager::Create()
{
	TSharedRef<SDPackageManager> PackageRef = MakeShareable(new SDPackageManager());
	return PackageRef;
}

void SDPackageManager::UpdateHovered(FVector2D MousePos, FGeometry PackGeo)
{
	// 获取悬停容器
	TSharedPtr<SSDContainerBaseWidget> CurrHoveredCon = LocateContainer(MousePos, PackGeo);

	// 如果容器存在
	if (CurrHoveredCon.IsValid()) {
		// 设置当前容器悬停显示
		CurrHoveredCon->UpdateHovered(true);
		// 如果上一容器存在且与当前不同则更新
		if (LastHoveredCon.IsValid() && LastHoveredCon.Get() != CurrHoveredCon.Get()) {
			// 更新悬停显示
			LastHoveredCon->UpdateHovered(false);
		}
	}
	else {
		if (LastHoveredCon.IsValid()) {
			LastHoveredCon->UpdateHovered(false);
		}
	}

	// 更新上一悬停容器
	LastHoveredCon = CurrHoveredCon;
}

void SDPackageManager::LeftOption(FVector2D MousePos, FGeometry PackGeo)
{
	// 先获取点击的容器
	TSharedPtr<SSDContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	// 如果容器存在，执行容器事件
	if (ClickedContainer.IsValid()) {
		ClickedContainer->LeftOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
	// 如果容器不存在，且鼠标手持物品，则丢弃物品
	if (!ClickedContainer.IsValid() && ObjectIndex != 0) {
		// 丢弃物品
		ThrowObject(ObjectIndex, ObjectNum);
		// 重置容器
		ObjectIndex = ObjectNum = 0;
	}
}

void SDPackageManager::RightOption(FVector2D MousePos, FGeometry PackGeo)
{
	// 先获取点击的容器
	TSharedPtr<SSDContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	// 如果容器存在，则执行事件
	if (ClickedContainer.IsValid()) {
		ClickedContainer->RightOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
}

TSharedPtr<SSDContainerBaseWidget> SDPackageManager::LocateContainer(FVector2D MousePos, FGeometry PackGeo)
{
	// 如果窗口分辨率非1920x1080则会出错，需在控制台窗口设置("-res 1920x1080")
	// 迭代找到指向容器（快捷栏、背包、合成窗口）
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It) {
		// 获取单个容器区域(左上和右下)
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);	 // 使用的是绝对大小
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
			return *It;
		}
	}

	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It) {
		// 获取单个容器区域(左上和右下)
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);	 // 使用的是绝对大小
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
			return *It;
		}
	}

	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		// 获取单个容器区域(左上和右下)
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);	 // 使用的是绝对大小
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
			return *It;
		}
	}

	// 获取单个容器区域(左上和右下)
	FVector2D StartPos = PackGeo.AbsoluteToLocal((OutputContainer)->GetCachedGeometry().AbsolutePosition);
	FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);	 // 使用的是绝对大小
	if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
		return OutputContainer;
	}

	return nullptr;
}

void SDPackageManager::ThrowObject(int ObjectID, int Num)
{
	PlayerThrowObject.ExecuteIfBound(ObjectID, Num);
}

void SDPackageManager::PackageShortChange(int ShortcutID, int ObjectID, int Num)
{
	// 执行手持物品修改委托
	ChangeHandObject.ExecuteIfBound(ShortcutID, ObjectID, Num);
}

void SDPackageManager::CompoundOutput(int ObjectID, int Num)
{
	// 如果获取的生成物为空，直接return
	if (ObjectID == 0) {
		return;
	}

	// 合成表结构数组
	TArray<int> TableMap;
	// 获取合成表
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		TableMap.Add((*It)->GetIndex());
	}
	TableMap.Add(ObjectID);

	// 消耗数量数组
	TArray<int> ExpendMap;
	// 遍历合成表图，找到对应合成表进行扣除
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SDDataHandle::Get()->CompoundTableMap); It; ++It) {
		// 如果找到符合的直接跳出循环
		if ((*It)->DetectExpend(&TableMap, Num, ExpendMap)) {
			break;
		}
	}

	// 扣除合成台物品数量
	for (int i = 0; i < 9; i++) {
		if (ExpendMap[i] != 0) {
			int InputNum = InputContainerList[i]->GetNum() - ExpendMap[i];
			int InputID = InputNum == 0 ? 0 : InputContainerList[i]->GetIndex();
			InputContainerList[i]->ResetContainerPara(InputID, InputNum);
		}
	}

}

void SDPackageManager::CompoundInput()
{
	// 获取合成台9个容器的物品ID和数量
	TArray<int> IDMap;
	TArray<int> NumMap;
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		IDMap.Add((*It)->GetIndex());
		NumMap.Add((*It)->GetNum());
	}

	// 合成输出结果
	int OutputIndex = 0;
	int OutputNum = 0;
	// 逐合成表检测是否可合成
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SDDataHandle::Get()->CompoundTableMap); It; ++It) {
		(*It)->DetectTable(&IDMap, &NumMap, OutputIndex, OutputNum);
		// 如果检测出一个可合成，就直接结束
		if (OutputIndex != 0) {
			break;
		}
	}

	// 对OutputContainer进行修改
	if (MultiplyAble(OutputIndex)) {
		// 如果可叠加则直接修改
		OutputContainer->ResetContainerPara(OutputIndex, OutputNum);
	}
	else {
		// 不可叠加则只输出一个
		OutputContainer->ResetContainerPara(OutputIndex, 1);
	}
}

bool SDPackageManager::MultiplyAble(int ObjectID)
{
	// 获取物品属性
	TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	// 返回是否是武器或工具
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}

bool SDPackageManager::SearchFreeSpace(int ObjectID)
{
	TSharedPtr<SSDContainerBaseWidget> FreeContainer;
	return SearchFreeSpace(ObjectID, FreeContainer);
}

bool SDPackageManager::SearchFreeSpace(int ObjectID, TSharedPtr<SSDContainerBaseWidget>& FreeContainer)
{
	// 空容器引用
	TSharedPtr<SSDContainerBaseWidget> EmptyContainer;
	// 先搜索快捷栏是否有该物品的叠加
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It) {
		// 如果空容器无引用且该容器为空则指向该容器
		if (!EmptyContainer.IsValid() && (*It)->IsEmpty()) {
			EmptyContainer = *It;
		}
		// 如果容器不为空，且与输入物品ID相同且可叠加，则直接放入
		if (!FreeContainer.IsValid() && (*It)->RemainSpace(ObjectID)) {
			FreeContainer = *It;
			return true;
		}
	}

	// 在搜索背包是否有该物品的叠加
	for (TArray<TSharedPtr<SSDContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It) {
		// 如果空容器无引用且该容器为空则指向该容器
		if (!EmptyContainer.IsValid() && (*It)->IsEmpty()) {
			EmptyContainer = *It;
		}
		// 如果容器不为空，且与输入物品ID相同且可叠加，则直接放入
		if (!FreeContainer.IsValid() && (*It)->RemainSpace(ObjectID)) {
			FreeContainer = *It;
			return true;
		}
	}

	// 如果没有叠加则尝试返回一个空容器
	if (EmptyContainer.IsValid()) {
		FreeContainer = EmptyContainer;
		return true;
	}

	return false;
}

void SDPackageManager::AddObject(int ObjectID)
{
	TSharedPtr<SSDContainerBaseWidget> FreeContainer;
	if (SearchFreeSpace(ObjectID, FreeContainer)) {
		// 添加物品到容器
		FreeContainer->AddObject(ObjectID);
	}
}

bool SDPackageManager::EatUpEvent(int shortcutID)
{
	// 获取物品属性
	TSharedPtr<ObjectAttribute> ObjectAttr = *SDDataHandle::Get()->ObjectAttrMap.
		Find(ShortcutContainerList[shortcutID]->GetIndex());
	// 如果是食物则吃下
	if (ObjectAttr->ObjectType == EObjectType::Food) {
		int NewNum = ShortcutContainerList[shortcutID]->GetNum() - 1;
		int NewIndex = NewNum == 0 ? 0 : ShortcutContainerList[shortcutID]->GetIndex();
		ShortcutContainerList[shortcutID]->ResetContainerPara(NewIndex, NewNum);
		return true;
	}

	return false;
}
