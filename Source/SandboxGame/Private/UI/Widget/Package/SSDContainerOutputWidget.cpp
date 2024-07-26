// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Package/SSDContainerOutputWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSDContainerOutputWidget::Construct(const FArguments& InArgs)
{
	SSDContainerBaseWidget::Construct(
		SSDContainerBaseWidget::FArguments()
		.WorkIndex(InArgs._WorkIndex)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSDContainerOutputWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	// ���������ƷΪ�գ����޷�������������У�����ɶ�ͷ���ɶ��
	if (ObjectIndex == 0) {
		OutputID = InputID;
		OutputNum = InputNum;
		return;
	}

	// ������뱾�طǿգ�����������Ʒ��ͬ�ҿɵ�������ȫ�����ӵ����
	if (InputID == ObjectIndex && MultiplyAble(InputID)) {
		OutputID = ObjectIndex;
		if (InputNum + ObjectNum <= 64) {
			OutputNum = InputNum + ObjectNum;
			ObjectNum = 0;
		}
		else {
			OutputNum = 64;
			ObjectNum = (InputNum + ObjectNum) - 64;
		}
		// ִ�кϳ����ί��(���ٺϳɲ���)
		CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum - InputNum);
		return;
	}

	// �����Ʒ����ͬ������ͬ���ܵ��ӣ������Ʒ����������
	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
	// �ϳ����ί��
	CompoundOutput.ExecuteIfBound(ObjectIndex, ObjectNum);
	// ִ�ж�����Ʒί��
	ThrowObject.ExecuteIfBound(InputID, InputNum);
}

void SSDContainerOutputWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	// ���������ƷΪ�գ����޲���
	if (ObjectIndex == 0) {
		OutputID = InputID;
		OutputNum = InputNum;
		return;
	}

	// �������Ϊ�գ���ֱ��������ص�һ��
	if (InputID == 0) {
		OutputID = ObjectIndex;
		OutputNum = (ObjectNum + 1) / 2;
		// ִ�кϳ����ί��
		CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum);
		// ��������

		return;
	}

	// ���������Ʒ��������Ʒ��ͬ�ҿɺϲ��������һ��
	if (InputID == ObjectIndex && MultiplyAble(InputID)) {
		OutputID = ObjectIndex;
		int num = FMath::Min((ObjectNum + 1) / 2, 64 - InputNum);
		OutputNum += num;
		ObjectNum -= num;
		// ִ�кϳ����ί��
		CompoundOutput.ExecuteIfBound(ObjectIndex, num);
		return;
	}

	// ���������Ʒ��������Ʒ����ͬ,����������Ʒ�����������Ʒ��һ��
	OutputID = ObjectIndex;
	OutputNum = (ObjectNum + 1) / 2;
	// ִ�кϳ�ί��
	CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum);
	// ִ�ж���ί��
	ThrowObject.ExecuteIfBound(InputID, InputNum);

}


