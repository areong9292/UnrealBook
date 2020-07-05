// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include "ABAnimInstance.h"

// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	if (SpringArm != NULL && Camera != NULL)
	{
		// �ٸ� ������Ʈ���� ĸ�� �ؿ� ������
		SpringArm->SetupAttachment(GetCapsuleComponent());

		// ī�޶�� ������ �� �ؿ� ������
		Camera->SetupAttachment(SpringArm);

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
	}

	// ��Ʈ�� ��带 ��ƺ�� ���� ����
	SetControlMode(EControlMode::DIABLO);

	// ����, ȸ�� ��ȭ �ӵ� ����
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// ���� ���� ���� - ĳ���� �����Ʈ ������Ʈ�� �ִ�
	GetCharacterMovement()->JumpZVelocity = 800.0f;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	// �޽÷κ��� �ִ� �ν��Ͻ�(�ִϸ��̼� �������Ʈ)�� �����´�
	auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	// �������Ʈ���� ��Ÿ�ָ� �÷����϶�� �Ѵ�
	AnimInstance->PlayAttackMontage();
}