// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolPos.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	// �½�ũ�� �̸��� �ٸ��̸����� ǥ��
	NodeName = TEXT("FindPatrolPos");
}

// �����̺�� Ʈ���� �½�ũ ���� �� �½�ũ Ŭ������ ExecuteTask �޼ҵ带 ȣ���Ѵ�
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// ���� �����̺�� Ʈ���� ���� ���� ���� �����´�
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		// �½�ũ ���������� ���� ����
		return EBTNodeResult::Failed;
	}

	// �׺�޽� �����´�
	UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		// �½�ũ ���������� ���� ����
		return EBTNodeResult::Failed;
	}

	// ���� ���� ��ġ �����´�
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey);

	// ���� ������ ��ġ�� �������� �����´�
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		// �����忡 �̾Ƴ� ���� ������ ���� �����Ѵ�
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatrolPosKey, NextPatrol.Location);

		// �½�ũ ���������� �����ߴ� ����
		return EBTNodeResult::Succeeded;
	}

	ABLOG(Warning, TEXT("Actor : %s"), *ControllingPawn->GetName());
	ABLOG(Warning, TEXT("Next Location : %s"), *NextPatrol.Location.ToString());
	// �½�ũ ���������� ���� ����
	return EBTNodeResult::Failed;
}
