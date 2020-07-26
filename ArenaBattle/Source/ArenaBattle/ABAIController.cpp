// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAIController.h"

AABAIController::AABAIController()
{
	RepeatInterval = 3.0f;
}

void AABAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	// Ÿ�̸� ����
	// RepeatInterval �ʸ���
	// AABAIController::OnRepeatTimer ȣ��
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AABAIController::OnRepeatTimer, RepeatInterval, true);
}

void AABAIController::UnPossess()
{
	Super::UnPossess();

	// Ÿ�̸� ����
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AABAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	ABCHECK(CurrentPawn != nullptr);

	// �׺���̼� �ý��� �����´�
	UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	FNavLocation NextLocation;
	// GetRandomPointInNavigableRadius - �̵� ������ �������� �������� �����´�
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		// SimpleMoveToLocation - �������� ���� �̵���Ų��
		UNavigationSystem::SimpleMoveToLocation(this, NextLocation.Location);
		ABLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}
}
