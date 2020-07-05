// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	// 현재 폰의 속도
	CurrentPawnSpeed = 0.0f;
	
	// 폰이 공중에 있는지 여부
	IsInAir = false;

	// 몽타주 관련 함수들은 항상 몽타주 에셋을 참조하므로 생성자에서 미리 불러둔다
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

// 매 틱마다 호출된다
void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			// 캐릭터 무브먼트 컴포넌트에서 IsFalling - 공중에 떠있는지 여부 를 저장한다
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	// 실행한다
	Montage_Play(AttackMontage, 1.0f);
}
