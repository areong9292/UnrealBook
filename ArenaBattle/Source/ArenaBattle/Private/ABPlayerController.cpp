// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"

// �÷��̾� ��Ʈ�ѷ��� �÷��̾��� �Է°� ��µǴ� ȭ���� å������

// ���Ͱ� ������ ��� ������Ʈ���� ���� �� ȣ���
void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// ���Ϳ� ��Ʈ�ѷ��� �����ϸ� ȣ���
void AABPlayerController::Possess(APawn * aPawn)
{
	ABLOG_S(Warning);
	Super::Possess(aPawn);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// �Է��� UI�� �����ϰ� ���ӿ��� �����ϰ� ����
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
