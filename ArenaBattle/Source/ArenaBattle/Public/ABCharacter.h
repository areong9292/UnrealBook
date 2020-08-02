// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

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

	// ī�޶� ��� enum
	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC
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

	// ��� ������Ʈ���� �ʱ�ȭ �� �� ȣ���
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PossessedBy(AController* NewController) override;

	bool CanSetWeapon();

	void SetWeapon(class AABWeapon* NewWeapon);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class AABWeapon* CurrentWeapon;
	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		UCameraComponent* Camera;

	// ���� ����
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UABCharacterStatComponent* CharacterStat;

	// ���� ����
	// ���Ϳ� UI ���� �� �ְ� UWidgetComponent�� �ִ�
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	// AI ��Ʈ�ѷ������� ������ �����ϵ��� public���� �̵�
	void Attack();

	// ���� ���� ��������Ʈ
	FOnAttackEndDelegate OnAttackEnd;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();

	// �ִ� �ν��Ͻ��� �ִϸ��̼� ��Ÿ�� ����� ������ ȣ��Ǵ� OnMontageEnded��� ��������Ʈ�� �����Ѵ�
	// �̸� ����ϱ� ���ؼ� UAnimMontage*, bool ���ڸ� ���� ��� �Լ��� ������ �ȴ�
	UFUNCTION()
		void OnAttackMontageEneded(UAnimMontage* Montage, bool bInterrupted);

	// ���� ����, ���� �� �� �����ϴ� �޼ҵ�
	void AttackStartComboState();
	void AttackEndComboState();

	// ���� üũ�ϴ� �޼ҵ�
	void AttackCheck();

private:
	// ���� ������ ���� ����� bool ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	// ���� �޺����� �̵����� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	// �޺� �Է� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	// ���� �������� �޺� ī����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	// �޺� ī���� �ƽ�ġ
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	// ���� ����(class)
	UPROPERTY()
		class UABAnimInstance* ABAnim;

	// ���� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	// ���� �ݰ�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

};