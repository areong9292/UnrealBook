// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 카메라 모드 enum
	enum class EControlMode
	{
		GTA,
		DIABLO
	};
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	void SetControlMode(EControlMode NewControlMode);

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 모든 컴포넌트들이 초기화 된 후 호출됨
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm = NULL;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		UCameraComponent* Camera = NULL;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();
	void Attack();

	// 애님 인스턴스는 애니메이션 몽타주 재생이 끝나면 호출되는 OnMontageEnded라는 델리게이트를 제공한다
	// 이를 등록하기 위해선 UAnimMontage*, bool 인자를 가진 멤버 함수가 있으면 된다
	UFUNCTION()
		void OnAttackMontageEneded(UAnimMontage* Montage, bool bInterrupted);

private:
	// 공격 중인지 여부 저장용 bool 변수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	// 전방 선언(class)
	UPROPERTY()
		class UABAnimInstance* ABAnim;
};
