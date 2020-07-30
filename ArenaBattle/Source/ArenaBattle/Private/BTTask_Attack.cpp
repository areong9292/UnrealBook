// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "ABAIController.h"
#include "ABCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	// tick 활성화
	bNotifyTick = true;

	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI가 조정 중인 폰(캐릭터)를 가져온다
	auto ABCharacter = Cast<AABCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ABCharacter == nullptr)
		return EBTNodeResult::Failed;

	// 태스크 실행 시 공격 진행
	ABCharacter->Attack();
	IsAttacking = true;

	// 공격 종료 델리게이트에 공격 중 플래그 오프
	ABCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 공격 중이 아닐 경우에만 태스크 종료
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
