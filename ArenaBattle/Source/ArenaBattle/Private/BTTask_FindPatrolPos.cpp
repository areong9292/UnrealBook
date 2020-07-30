// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolPos.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	// 태스크의 이름을 다른이름으로 표시
	NodeName = TEXT("FindPatrolPos");
}

// 비헤이비어 트리는 태스크 실행 시 태스크 클래스의 ExecuteTask 메소드를 호출한다
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 현재 비헤이비어 트리로 조작 중인 폰을 가져온다
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		// 태스크 수행했지만 실패 리턴
		return EBTNodeResult::Failed;
	}

	// 네비메시 가져온다
	UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		// 태스크 수행했지만 실패 리턴
		return EBTNodeResult::Failed;
	}

	// 폰의 시작 위치 가져온다
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey);

	// 다음 움직일 위치를 랜덤으로 가져온다
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		// 블랙보드에 뽑아낸 다음 움직일 값을 셋팅한다
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatrolPosKey, NextPatrol.Location);

		// 태스크 성공적으로 수행했다 리턴
		return EBTNodeResult::Succeeded;
	}

	ABLOG(Warning, TEXT("Actor : %s"), *ControllingPawn->GetName());
	ABLOG(Warning, TEXT("Next Location : %s"), *NextPatrol.Location.ToString());
	// 태스크 수행했지만 실패 리턴
	return EBTNodeResult::Failed;
}
