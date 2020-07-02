// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	// 현재 폰의 속도
	CurrentPawnSpeed = 0.0f;
	
	// 폰이 공중에 있는지 여부
	IsInAir = false;
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
