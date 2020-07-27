// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	// �̸��� Detect�� �ϰ� 1�ʿ� �ѹ��� TickNode �ݺ��Ѵ�
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}
void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// ���� AI�� ���� ���� ���� �����´�
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;
	
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (World == nullptr)
		return;

	// �ݰ� 6���� �浹üũ
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,									// �浹 üũ ����� ���⿡ ����
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// �ݰ� ���� ĳ���͸� ã���� ���
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			// �浹 üũ �� ��� ABCharacter(�� ������ Ÿ�ٿ� ABCharcter�� ��������) �߿���
			// �÷��̾� ��Ʈ�ѷ��� �������� ����(�÷��̾�)�� ���
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			if (ABCharacter != nullptr && ABCharacter->GetController()->IsPlayerController())
			{
				// �� ������ target�� ã�� �÷��̾� ���� �ְ�
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);

				// ����� �׸� �׸���
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

	// �ݰ� 6���� ������ �׸� �׸���
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
