// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"

// 플레이어 컨트롤러는 플레이어의 입력과 출력되는 화면을 책임진다

// 액터가 소유한 모든 컴포넌트들이 셋팅 후 호출됨
void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// 액터에 컨트롤러가 빙의하면 호출됨
void AABPlayerController::Possess(APawn * aPawn)
{
	ABLOG_S(Warning);
	Super::Possess(aPawn);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력을 UI를 배제하고 게임에만 전달하게 셋팅
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
