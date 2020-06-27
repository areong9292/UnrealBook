// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"


// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	if (SpringArm != NULL && Camera != NULL)
	{
		// 다른 컴포넌트들은 캡슐 밑에 붙힌다
		SpringArm->SetupAttachment(GetCapsuleComponent());

		// 카메라는 스프링 암 밑에 붙힌다
		Camera->SetupAttachment(SpringArm);

		// 캡슐은 가운데 기준, 리소스는 발 기준이기 때문에 절반 정도 위치 수정
		// 회전은 언리얼 엔진과 리소스 만든 좌표계가 달라서 -90도 돌려서 맞춰줌
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

		// 스프링암 길이 400, x축으로 -15도 돌려서 위에서 대각선으로 바라보게 함
		SpringArm->TargetArmLength = 400.0f;
		SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

		// 에셋 경로로 에셋 부르기
		static ConstructorHelpers::FObjectFinder<USkeletalMesh>
			SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard"));

		// 부르기 성공했으면
		if (SK_CARDBOARD.Succeeded())
		{
			// SetSkeletalMesh에 에셋의 포인터(SK_CARDBOARD.Object)를 전달한다
			GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
		}

		// 애니메이션 블루프린트로 애니메이션 실행
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
		if (WARRIOR_ANIM.Succeeded())
		{
			// 스켈레탈 메시 컴포넌트의 애님 인스턴스 속성에
			// 애니메이션 블루프린트의 클래스 정보를 지정한다
			// 애님 인스턴스 보고 이 애니메이션 블루프린트를 실행해라고 하는 것
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
	//GetActorForwardVector - 액터의 전진방향 벡터
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	//GetActorForwardVector - 액터의 우측방향 벡터
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

