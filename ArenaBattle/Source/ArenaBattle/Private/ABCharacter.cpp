// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "ABWeapon.h"
#include "DrawDebugHelpers.h"
#include "ABCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"
#include "ABAIController.h"
#include "ABCharacterSetting.h"

// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	// �ٸ� ������Ʈ���� ĸ�� �ؿ� ������
	SpringArm->SetupAttachment(GetCapsuleComponent());

	// ī�޶�� ������ �� �ؿ� ������
	Camera->SetupAttachment(SpringArm);

	// UI �޽ÿ� ������
	HPBarWidget->SetupAttachment(GetMesh());

	// ĸ���� ��� ����, ���ҽ��� �� �����̱� ������ ���� ���� ��ġ ����
	// ȸ���� �𸮾� ������ ���ҽ� ���� ��ǥ�谡 �޶� -90�� ������ ������
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// �������� ���� 400, x������ -15�� ������ ������ �밢������ �ٶ󺸰� ��
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// ���� ��η� ���� �θ���
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard"));

	// �θ��� ����������
	if (SK_CARDBOARD.Succeeded())
	{
		// SetSkeletalMesh�� ������ ������(SK_CARDBOARD.Object)�� �����Ѵ�
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// �ִϸ��̼� �������Ʈ�� �ִϸ��̼� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		// ���̷�Ż �޽� ������Ʈ�� �ִ� �ν��Ͻ� �Ӽ���
		// �ִϸ��̼� �������Ʈ�� Ŭ���� ������ �����Ѵ�
		// �ִ� �ν��Ͻ� ���� �� �ִϸ��̼� �������Ʈ�� �����ض�� �ϴ� ��
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);

		// ���̷�Ż �޽� ������Ʈ�� �ִϸ��̼� ��� ����Ʈ Ŭ���� ������ ����ϸ�
		// ������Ʈ�� Ŭ���� ������ �ν��Ͻ��� ����, �ִϸ��̼��� �����ϵ��� �Ѵ�
	}

	// ĳ���� ���� ��ġ�ϰ� ��
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	// �׻� �÷��̾ �ٶ󺸰� Screen���� ����
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// UI �������Ʈ �����´�
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);

		// UI ������� ������ ũ��� ����
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	// ��Ʈ�� ��带 ��ƺ�� ���� ����
	SetControlMode(EControlMode::DIABLO);

	// ����, ȸ�� ��ȭ �ӵ� ����
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// ���� ���� ���� - ĳ���� �����Ʈ ������Ʈ�� �ִ�
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;

	// �޺� �ƽ�ġ ���� ��
	// �޺� ���� ������ �ʱ�ȭ
	MaxCombo = 4;
	AttackEndComboState();

	// �ݸ��� ������ ����
	// ĸ�� ������Ʈ�� ���� ���� ABCharacter�� ���������� ����ϰ� �Ѵ�
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// ���� ����, �ݰ��� �����Ѵ�
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	/*
	// ���� ������ �ϴ� ���� �̸� ������ ����
	// ������ ���̷�Ż �޽� ������Ʈ�� ���̷�Ż �޽ÿ� �ִ�
	// GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object); ��������
	// ���� ���� �� ���� �ִ��� üũ�ؾ� �Ѵ�
	FName WeaponSocket(TEXT("hand_rSocket"));

	// �޽� �ȿ� �ش� ������ �ִ��� üũ
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		// ���� ���� �ҷ��´�
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}

		// ���⸦ �޽ÿ� �����µ� ���ڷ� ���� �̸�(WeaponSocket)�� �����ϸ�
		// ���� ��ġ�� �������� Ʈ���� ���� �ڵ����� �����ȴ�
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	*/

	// ABAIController �ΰ����� ����
	AIControllerClass = AABAIController::StaticClass();

	// �÷��̾ �����ϴ� ĳ���͸� ������ ���
	// ������ ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� ĳ���ʹ� �ڵ����� ABAIController�� ������
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	auto DefaultSetting = GetDefault<UABCharacterSetting>();
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset : DefaultSetting->CharacterAssets)
		{
			ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
		}
	}
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*
	FName WeaponSocket(TEXT("hand_rSocket"));

	// ���͸� ���忡 �����Ѵ�
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon != nullptr)
	{
		// �� �� ������ ���͸� ĳ������ WeaponSocket�� ã�Ƽ� �ű⿡ ������
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	*/
}

void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	if (SpringArm != NULL)
	{
		switch(CurrentControlMode)
		{
			case EControlMode::GTA:
				//SpringArm->TargetArmLength = 450.0f;
				//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
				// ������ �� ���̸� 450���� ����
				ArmLengthTo = 450.0f;

				// �÷��̾� ��Ʈ�ѷ��� ���� ȸ�� ���� �������� ��
				SpringArm->bUsePawnControlRotation = true;
				SpringArm->bInheritPitch = true;
				SpringArm->bInheritRoll = true;
				SpringArm->bInheritYaw = true;

				// ī�޶� ��ֹ� ������ ����
				SpringArm->bDoCollisionTest = true;

				// �⺻������ ��Ʈ�ѷ��� yawȸ���� ���� yaw ȸ���� �����Ǿ��ִ�
				// false �̹Ƿ� ��Ʈ�ѷ��� �����δ�
				bUseControllerRotationYaw = false;

				// ĳ���Ͱ� �����̴� �������� ĳ���͸� �ڵ����� ȸ����Ų��
				GetCharacterMovement()->bOrientRotationToMovement = true;

				// ȸ���ӵ�
				GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

				// �ε巴�� ȸ�� ����
				GetCharacterMovement()->bUseControllerDesiredRotation = false;

				break;

			case EControlMode::DIABLO:
				//SpringArm->TargetArmLength = 800.0f;
				//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
				// ������ �� ���̸� 450���� ����
				// ������ �� ȸ�� ���� x������ -45�� ����
				ArmLengthTo = 800.0f;
				ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);

				SpringArm->bUsePawnControlRotation = false;
				SpringArm->bInheritPitch = false;
				SpringArm->bInheritRoll = false;
				SpringArm->bInheritYaw = false;

				SpringArm->bDoCollisionTest = false;

				bUseControllerRotationYaw = false;

				// ĳ���Ͱ� �����̴� �������� ĳ���͸� �ڵ����� ȸ����Ų��
				GetCharacterMovement()->bOrientRotationToMovement = false;

				// ȸ���ӵ�
				GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

				// �ε巴�� ȸ�� ����
				GetCharacterMovement()->bUseControllerDesiredRotation = true;

				break;

			case EControlMode::NPC:
				bUseControllerRotationYaw = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = false;
				GetCharacterMovement()->bOrientRotationToMovement = true;
				GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
				break;
		}
	}
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �� ƽ���� ������ ���� ���̸� ��� ���濡 �°� �������ش�
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	// ���� ��尡 ��ƺ�� �� ���, ȸ���� �������ش�
	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		break;
	case EControlMode::DIABLO:
		// SizeSquared - �ش� ������ ���� ���ϱ�
		// ���� ���̰� 0���� ũ��
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// ��Ʈ�ѷ��� ȸ�� ���� �ش� ���� x�κ��� ȸ�� ��� ������ ������ �����Ѵ�
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());

			AddMovementInput(DirectionToMove);
		}
		break;
	}

}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(ABAnim != nullptr);

	// �ִ� �ν��Ͻ�(�ִϸ��̼� �������Ʈ)�� OnMontageEnded ��������Ʈ�� OnAttackMontageEneded�� �����Ѵ�
	// �׷��� ��Ÿ�� ��� ���� �� OnAttackMontageEneded ���� ��
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEneded);

	// �ִ� �ν��Ͻ��� OnNextAttackCheck ��������Ʈ�� ���� ���

	// = �� ������ ������ �� ���� ���� ��û�� �� �����
	// ���� �޺��� �Ѿ��
	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		// ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		// �޺� �Է��� ������ ��� == ���� ���� ��û�� �� ���
		if (IsComboInputOn)
		{
			// �޺� �� ���� ��
			AttackStartComboState();

			// �� �޺� ���¿��� ���� ���·� �Ѿ��
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	// �ִ� �ν��Ͻ��� OnAttackHitCheck ��������Ʈ�� AttackCheck ���� ���
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	// ��������Ʈ ����
	// �ش� ��������Ʈ�� ȣ��Ǹ� ���� ó���Ѵ�
	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnHPIsZero"));
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});

	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);

	// IE_Pressed - ������ ��� ȣ��
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);

	// ��ӹ��� ACharacter�� ���� ����� �̹� �ִ�
	// ���ε��� ���ָ� �۵���
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	
	// IE_Pressed - ������ ��� ȣ��
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

// ������ ������ ȣ��ȴ�
float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// �θ� Ŭ������ AActor�� �⺻���� ������ ���� ������ �����Ǿ� �ִ�
	// ����� ���� �������� ��´�
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	/*
	// �������� ������ ���
	if (FinalDamage > 0.0f)
	{
		// �״� �ִϸ��̼� ����� ���� �ִ� �ν��Ͻ�(�ִϸ��̼� �������Ʈ)��
		// IsDead �÷��׸� Ų��
		ABAnim->SetDeadAnim();

		// ���� �ݸ����� �������� �浹 �̺�Ʈ�� �� �̻� �߻����� �ʰ� �Ѵ�
		SetActorEnableCollision(false);
	}
	*/

	// ������ ó���� ���� ������Ʈ���� �Ѵ�
	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void AABCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		ABLOG(Warning, TEXT("1234Actor : %s"), *GetName());
		SetControlMode(EControlMode::DIABLO);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	// NPC�� ��� ȸ���� �ڿ������� ���� ������ �� �ӵ��� ���δ�
	else
	{
		ABLOG(Warning, TEXT("5678Actor : %s"), *GetName());
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

// ���� ���� �������� üũ
bool AABCharacter::CanSetWeapon()
{
	return (CurrentWeapon == nullptr);
}

// ���� ����
void AABCharacter::SetWeapon(AABWeapon * NewWeapon)
{
	ABCHECK(NewWeapon != nullptr && CurrentWeapon == nullptr);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if(NewWeapon != nullptr)
	{
		// ���⸦ �����ϰ� ĳ���Ͱ� �� ���⸦ �����ϰ� �Ѵ�
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void AABCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// FRotationMatrix(GetControlRotation()) - ��Ʈ���� ȸ�� ����� �����´�
		// .GetUnitAxis(EAxis::X) X���� �����ͼ� �ش� ���� normal ���͸� �����´�
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

void AABCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

void AABCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}

void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// ��Ʈ�ѷ��� ȸ���� ������ ȸ�� ��(ĳ���� ȸ�� ������ ����)
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		// ��Ʈ�ѷ��� ȸ���� ������ ���� ȸ�� ������ ����
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::Attack()
{
	// ���� ���� ���� ���� ���¿��� ���� ��û�� ���� ���
	if (IsAttacking)
	{
		// �� �޺� ���� 1�̻� MaxCombo ���� �� ���
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));

		// ���� �޺��� �� �� ������ �޺� �Է��� ���Դٴ� �÷��׸� Ų��
		if(CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	// ���ο� ������ �����̸�
	else
	{
		// ���� �޺����� üũ
		ABCHECK(CurrentCombo == 0);

		// ���� �� ����, ���� �޺� �� ��������
		AttackStartComboState();
		
		// ���� ����Ű�� ������ �÷��� ��Ų�� - ó�� ��
		ABAnim->PlayAttackMontage();

		// AttackStartComboState �������� ������ ���� �޺� ������
		// ���� �������� �Ѿ��
		ABAnim->JumpToAttackMontageSection(CurrentCombo);

		// ���� ���� ���̶�� �÷��� ��
		IsAttacking = true;
	}
}

// ���� �� ��������Ʈ�� ���� ���� �ִϸ��̼� ����� �Ϸ�Ǹ� ȣ��ȴ�
void AABCharacter::OnAttackMontageEneded(UAnimMontage * Montage, bool bInterrupted)
{
	// ���� ���̾����� üũ
	ABCHECK(IsAttacking);

	// �ϳ��� �����ߴ��� üũ
	ABCHECK(CurrentCombo > 0);

	// ���� �� �÷��� ����
	IsAttacking = false;

	// �޺��� �������Ƿ� �޺� ���� ���� �ʱ�ȭ
	AttackEndComboState();

	// �ִϸ��̼� ��� �Ϸ� �� ��������Ʈ ����
	OnAttackEnd.Broadcast();
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;

	// CurrentCombo ���� 0 �̻� MaxCombo - 1 �������� üũ
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));

	// CurrentCombo + 1�� 1�� MaxCombo �� ���̿� �׻� �����ϵ��� �����Ѵ�
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AABCharacter::AttackCheck()
{
	// ���� ������ ���Ͱ� Ž���� ��� ������ ��� �� ����ü
	// HitResult�� Actor�� �� ������(TWeakObjectPtr) ������� ����Ǿ��ִ�
	// TWeakObjectPtr�� ������ ���Ϳ� �����Ϸ��� IsValid�� ����Ϸ��� ���Ͱ� ��ȿ���� ���� �����Ͽ��� �Ѵ�
	// �𸮾󿡼� ����Ʈ ������ �����Ѱ�
	FHitResult HitResult;

	FCollisionQueryParams Params(NAME_None, false, this);

	// SweepSingleByChannel - ���������� �������� �����鼭 �浹�� �����Ѵ�
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),											// ������
		GetActorLocation() + GetActorForwardVector() * AttackRange,	// ����
		FQuat::Identity,											// Ž���� �� ������ ȸ��
		ECollisionChannel::ECC_GameTraceChannel2,					// Ž�� ä��
		FCollisionShape::MakeSphere(AttackRadius),					// Ž���� �� ��������
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;			// ĳ���� �ü� ���� ����
	FVector Center = GetActorLocation() + TraceVec * 0.5f;				// �߰� ����
	float HalfHeight = AttackRange * 0.5f + AttackRadius;				// �߰� ����
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();	// ĸ�� Z�� ĳ���� �ü� �������� ���ϰ� ȸ��
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;			// ���� ���� ���ο� ���� �÷� ����
	float DebugLifeTime = 5.0f;											// 5�� �� ����

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#endif

	// �浹�� �����Ǿ���
	if (bResult)
	{
		// ���� ������ ������
		if (HitResult.Actor.IsValid())
		{
			// �浹�� ���� �̸��� �α���´�
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			// AActor���� TakeDamage�� �����Ǿ��ִ�
			// ���� �޴� ���Ϳ� TakeDamage�� override�ϰ�
			// ���⼭ ȣ���ϸ� ���ݹ޴� ������ ó���� �� �ִ�

			// ���Ϳ� �������� �����Ѵ�
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(
				CharacterStat->GetAttack(),			// ������ �������� ���� - �����Ϳ��� �ҷ��� ������ �����Ѵ�
				DamageEvent,						// �������� ���� - ���⼭�� �⺻��
				GetController(),					// ���� ����� ���� ������ - �������� �����ϴ� ��ü�� ���� �����ϴ� �÷��̾� ��Ʈ�ѷ���!
				this);								// ������ ������ ���� ����� ���� - ������ ������ ���� ABCharacter��� ĳ����(��)�� ����Ѵ�!
		}
	}
}
