// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPawn.h"


// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Component 생성
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// 움직임을 담당할 캡슐 컴포넌트를 루트로 지정
	RootComponent = Capsule;

	// 다른 컴포넌트들은 캡슐 밑에 붙힌다
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);

	// 카메라는 스프링 암 밑에 붙힌다
	Camera->SetupAttachment(SpringArm);

	// 캐릭터가 캡슐 안으로 들어올 수 있도록 캐릭터 절반 높이 88, 몸둘레 34로 캡슐 셋팅
	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);

	// 캡슐은 가운데 기준, 리소스는 발 기준이기 때문에 절반 정도 위치 수정
	// 회전은 언리얼 엔진과 리소스 만든 좌표계가 달라서 -90도 돌려서 맞춰줌
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

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
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// 애니메이션 블루프린트로 애니메이션 실행
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
	// LoadObject - 게임 실행 중에 애셋을 로드한다
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/Book/Animations/WarriorRun.WarriorRun"));
	
	// 달리기 애니메이션을 실행한다
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

// 액터가 소유한 모든 컴포넌트들이 셋팅 후 호출됨
void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// 이 폰이 특정 컨트롤러에 의해 빙의 당했을 경우 호출됨
void AABPawn::PossessedBy(AController * NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}

void AABPawn::UpDown(float NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	//GetActorForwardVector - 액터의 전진방향 벡터
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AABPawn::LeftRight(float NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	//GetActorForwardVector - 액터의 우측방향 벡터
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}
