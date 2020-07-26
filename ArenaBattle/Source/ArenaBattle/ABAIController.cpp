// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAIController.h"

AABAIController::AABAIController()
{
	RepeatInterval = 3.0f;
}

void AABAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	// 타이머 셋팅
	// RepeatInterval 초마다
	// AABAIController::OnRepeatTimer 호출
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AABAIController::OnRepeatTimer, RepeatInterval, true);
}

void AABAIController::UnPossess()
{
	Super::UnPossess();

	// 타이머 해제
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AABAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	ABCHECK(CurrentPawn != nullptr);

	// 네비게이션 시스템 가져온다
	UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	FNavLocation NextLocation;
	// GetRandomPointInNavigableRadius - 이동 가능한 목적지를 랜덤으로 가져온다
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		// SimpleMoveToLocation - 목적지로 폰을 이동시킨다
		UNavigationSystem::SimpleMoveToLocation(this, NextLocation.Location);
		ABLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}
}
