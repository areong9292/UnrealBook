// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	// ���� ���� �ӵ�
	CurrentPawnSpeed = 0.0f;
	
	// ���� ���߿� �ִ��� ����
	IsInAir = false;

	// ��Ÿ�� ���� �Լ����� �׻� ��Ÿ�� ������ �����ϹǷ� �����ڿ��� �̸� �ҷ��д�
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

// �� ƽ���� ȣ��ȴ�
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
			// ĳ���� �����Ʈ ������Ʈ���� IsFalling - ���߿� ���ִ��� ���� �� �����Ѵ�
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	// �����Ѵ�
	Montage_Play(AttackMontage, 1.0f);
}

// ���� ��Ÿ�� �������� �Ѿ�� �޼ҵ�
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// ���� ���� ��Ÿ�ְ� ���� ������ üũ
	ABCHECK(Montage_IsPlaying(AttackMontage));

	// ���� ������ �̸��� �����ͼ�
	// �� �̸��� �������� �Ѿ��
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

// �ִϸ��̼� ��� �� ��Ƽ���̰� ������ �𸮾���
// �ִ� �ν��Ͻ� Ŭ������ 'AnimNotify_��Ƽ���̸�' �̶�� �̸��� ��� �Լ��� ȣ���Ѵ�
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	// Broadcast - ��Ƽĳ��Ʈ ��������Ʈ�� ��ϵ� ��� �Լ��� ȣ���Ѵ�
	// OnAttackHitCheck�� ��ϵ� ��� �Լ� ȣ��
	OnAttackHitCheck.Broadcast();
}

// �ִϸ��̼� ��� �� ��Ƽ���̰� ������ �𸮾���
// �ִ� �ν��Ͻ� Ŭ������ 'AnimNotify_��Ƽ���̸�' �̶�� �̸��� ��� �Լ��� ȣ���Ѵ�
void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	// Broadcast - ��Ƽĳ��Ʈ ��������Ʈ�� ��ϵ� ��� �Լ��� ȣ���Ѵ�
	// OnNextAttackCheck�� ��ϵ� ��� �Լ� ȣ��
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
