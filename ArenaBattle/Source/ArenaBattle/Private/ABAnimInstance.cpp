// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	// 현재 폰의 속도
	CurrentPawnSpeed = 0.0f;
	
	// 폰이 공중에 있는지 여부
	IsInAir = false;

	// 죽음 여부
	IsDead = false;

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

	// 폰이 유효하지 않으면 종료
	if (!::IsValid(Pawn)) return;

	// 죽은 상태가 아닌 경우 애니메이션을 위한 상태 체크
	if (!IsDead)
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
	// 죽은 상태가 아닌 경우에만 애니메이션 재생
	ABCHECK(!IsDead);

	// 실행한다
	Montage_Play(AttackMontage, 1.0f);
}

// 다음 몽타주 섹션으로 넘어가는 메소드
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// 죽은 상태가 아닌 경우에만 애니메이션 재생
	ABCHECK(!IsDead);

	// 현재 공격 몽타주가 실행 중인지 체크
	ABCHECK(Montage_IsPlaying(AttackMontage));

	// 다음 섹션의 이름을 가져와서
	// 그 이름의 섹션으로 넘어간다
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

// 애니메이션 재생 시 노티파이가 있으면 언리얼은
// 애님 인스턴스 클래스의 'AnimNotify_노티파이명' 이라는 이름의 멤버 함수를 호출한다
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	// Broadcast - 멀티캐스트 델리게이트에 등록된 모든 함수를 호출한다
	// OnAttackHitCheck에 등록된 모든 함수 호출
	OnAttackHitCheck.Broadcast();
}

// 애니메이션 재생 시 노티파이가 있으면 언리얼은
// 애님 인스턴스 클래스의 'AnimNotify_노티파이명' 이라는 이름의 멤버 함수를 호출한다
void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	// Broadcast - 멀티캐스트 델리게이트에 등록된 모든 함수를 호출한다
	// OnNextAttackCheck에 등록된 모든 함수 호출
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
