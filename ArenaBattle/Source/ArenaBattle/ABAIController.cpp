// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPos(TEXT("PatrolPos"));

AABAIController::AABAIController()
{
	// 블랙 보드 - AI 판단을 위해 쓰이는 데이터 집합
	// 블랙보드 셋팅
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_ABCharacter.BB_ABCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	// 비헤이비어 트리 - 블랙보드 데이터에 기반에 설계한 트리
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_ABCharacter.BT_ABCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

// 폰에 빙의한다
void AABAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	// Blackboard - AIController에 있는 UBlackboardComponent
	// UseBlackboard - Blackboard에 BBAsset 셋팅
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// 블랙보드에 현재 빙의한 폰 위치 값을 HomePosKey에 셋팅
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());

		// BBAsset데이터로 BTAsset 실행
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}
