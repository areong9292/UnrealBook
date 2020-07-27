// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	// 이름을 Detect로 하고 1초에 한번씩 TickNode 반복한다
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}
void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 현재 AI가 조정 중인 폰을 가져온다
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;
	
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (World == nullptr)
		return;

	// 반경 6미터 충돌체크
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,									// 충돌 체크 결과가 여기에 담긴다
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// 반경 내의 캐릭터를 찾았을 경우
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			// 충돌 체크 된 모든 ABCharacter(블랙 보드의 타겟에 ABCharcter로 한정지음) 중에서
			// 플레이어 컨트롤러로 조작중인 액터(플레이어)인 경우
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			if (ABCharacter != nullptr && ABCharacter->GetController()->IsPlayerController())
			{
				// 블랙 보드의 target에 찾은 플레이어 정보 넣고
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);

				// 디버깅 그림 그린다
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);

				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);
	}

	// 반경 6미터 디버깅용 그림 그리기
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
