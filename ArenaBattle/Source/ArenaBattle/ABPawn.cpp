// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPawn.h"


// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Component ����
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// �������� ����� ĸ�� ������Ʈ�� ��Ʈ�� ����
	RootComponent = Capsule;

	// �ٸ� ������Ʈ���� ĸ�� �ؿ� ������
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);

	// ī�޶�� ������ �� �ؿ� ������
	Camera->SetupAttachment(SpringArm);

	// ĳ���Ͱ� ĸ�� ������ ���� �� �ֵ��� ĳ���� ���� ���� 88, ���ѷ� 34�� ĸ�� ����
	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);

	// ĸ���� ��� ����, ���ҽ��� �� �����̱� ������ ���� ���� ��ġ ����
	// ȸ���� �𸮾� ������ ���ҽ� ���� ��ǥ�谡 �޶� -90�� ������ ������
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

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
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// �ִϸ��̼� �������Ʈ�� �ִϸ��̼� ����
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}
}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();

	/*
	// LoadObject - ���� ���� �߿� �ּ��� �ε��Ѵ�
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/Book/Animations/WarriorRun.WarriorRun"));
	
	// �޸��� �ִϸ��̼��� �����Ѵ�
	if (AnimAsset != nullptr)
	{
		Mesh->PlayAnimation(AnimAsset, true);
	}
	*/
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);
}

// ���Ͱ� ������ ��� ������Ʈ���� ���� �� ȣ���
void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// �� ���� Ư�� ��Ʈ�ѷ��� ���� ���� ������ ��� ȣ���
void AABPawn::PossessedBy(AController * NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}

void AABPawn::UpDown(float NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	//GetActorForwardVector - ������ �������� ����
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AABPawn::LeftRight(float NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	//GetActorForwardVector - ������ �������� ����
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}
