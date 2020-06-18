// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"

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
