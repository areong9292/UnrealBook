// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"

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