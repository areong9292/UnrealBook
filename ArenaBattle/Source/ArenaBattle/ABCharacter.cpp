// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"


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
		}
	}
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
}

void AABCharacter::UpDown(float NewAxisValue)
{
	//GetActorForwardVector - ������ �������� ����
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	//GetActorForwardVector - ������ �������� ����
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}

void AABCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AABCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

