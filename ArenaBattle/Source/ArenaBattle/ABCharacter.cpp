// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include "ABAnimInstance.h"

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

			// 스켈레탈 메시 컴포넌트에 애니메이션 블루 프린트 클래스 정보를 등록하면
			// 컴포넌트는 클래스 정보로 인스턴스를 생성, 애니메이션을 관리하도록 한다
		}
	}

	// 컨트롤 모드를 디아블로 모드로 변경
	SetControlMode(EControlMode::DIABLO);

	// 길이, 회전 변화 속도 지정
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// 점프 높이 조절 - 캐릭터 무브먼트 컴포넌트에 있다
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;

	// 콤보 맥스치 셋팅 후
	// 콤보 관련 변수들 초기화
	MaxCombo = 4;
	AttackEndComboState();
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
				// 스프링 암 길이를 450으로 지정
				ArmLengthTo = 450.0f;

				// 플레이어 컨트롤러와 같은 회전 값을 가지도록 함
				SpringArm->bUsePawnControlRotation = true;
				SpringArm->bInheritPitch = true;
				SpringArm->bInheritRoll = true;
				SpringArm->bInheritYaw = true;

				// 카메라를 장애물 앞으로 줌인
				SpringArm->bDoCollisionTest = true;

				// 기본적으로 컨트롤러의 yaw회전과 폰의 yaw 회전은 연동되어있다
				// false 이므로 컨트롤러만 움직인다
				bUseControllerRotationYaw = false;

				// 캐릭터가 움직이는 방향으로 캐릭터를 자동으로 회전시킨다
				GetCharacterMovement()->bOrientRotationToMovement = true;

				// 회전속도
				GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

				// 부드럽게 회전 여부
				GetCharacterMovement()->bUseControllerDesiredRotation = false;

				break;

			case EControlMode::DIABLO:
				//SpringArm->TargetArmLength = 800.0f;
				//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
				// 스프링 암 길이를 450으로 지정
				// 스프링 암 회전 값을 x축으로 -45도 지정
				ArmLengthTo = 800.0f;
				ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);

				SpringArm->bUsePawnControlRotation = false;
				SpringArm->bInheritPitch = false;
				SpringArm->bInheritRoll = false;
				SpringArm->bInheritYaw = false;

				SpringArm->bDoCollisionTest = false;

				bUseControllerRotationYaw = false;

				// 캐릭터가 움직이는 방향으로 캐릭터를 자동으로 회전시킨다
				GetCharacterMovement()->bOrientRotationToMovement = false;

				// 회전속도
				GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

				// 부드럽게 회전 여부
				GetCharacterMovement()->bUseControllerDesiredRotation = true;

				break;
		}
	}
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 매 틱마다 스프링 암의 길이를 모드 변경에 맞게 수정해준다
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	// 현재 모드가 디아블로 일 경우, 회전도 수정해준다
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
		// SizeSquared - 해당 벡터의 길이 구하기
		// 벡터 길이가 0보다 크면
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// 컨트롤러의 회전 값을 해당 벡터 x로부터 회전 행렬 가져온 값으로 지정한다
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

	// 애님 인스턴스(애니메이션 블루프린트)의 OnMontageEnded 델리게이트에 OnAttackMontageEneded를 연결한다
	// 그러면 몽타쥬 재생 끝난 후 OnAttackMontageEneded 실행 됨
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEneded);

	// 애님 인스턴스의 OnNextAttackCheck 델리게이트에 로직 등록

	// = 한 섹션이 끝났을 때 내가 공격 요청을 한 경우라면
	// 다음 콤보로 넘어간다
	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		// 콤보 입력이 들어왔을 경우 == 내가 공격 요청을 한 경우
		if (IsComboInputOn)
		{
			// 콤보 값 셋팅 후
			AttackStartComboState();

			// 현 콤보 상태에서 다음 상태로 넘어간다
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);

	// IE_Pressed - 눌렸을 경우 호출
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);

	// 상속받은 ACharacter에 점프 기능이 이미 있다
	// 바인딩만 해주면 작동함
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	
	// IE_Pressed - 눌렸을 경우 호출
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

void AABCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// FRotationMatrix(GetControlRotation()) - 컨트롤의 회전 행렬을 가져온다
		// .GetUnitAxis(EAxis::X) X값을 가져와서 해당 축의 normal 벡터를 가져온다
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
		// 컨트롤러의 회전을 액터의 회전 값(캐릭터 회전 값으로 지정)
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		// 컨트롤러의 회전을 스프링 암의 회전 값으로 지정
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::Attack()
{
	// 내가 현재 공격 중인 상태에서 공격 요청이 들어온 경우
	if (IsAttacking)
	{
		// 현 콤보 값이 1이상 MaxCombo 이하 인 경우
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));

		// 다음 콤보로 갈 수 있으면 콤보 입력이 들어왔다는 플래그를 킨다
		if(CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	// 새로운 공격의 시작이면
	else
	{
		// 시작 콤보인지 체크
		ABCHECK(CurrentCombo == 0);

		// 시작 값 셋팅, 다음 콤보 값 가져오기
		AttackStartComboState();
		
		// 현재 가리키는 섹션을 플레이 시킨다 - 처음 꺼
		ABAnim->PlayAttackMontage();

		// AttackStartComboState 실행으로 가져온 다음 콤보 값으로
		// 다음 섹션으로 넘어간다
		ABAnim->JumpToAttackMontageSection(CurrentCombo);

		// 현재 공격 중이라는 플래그 온
		IsAttacking = true;
	}
}

// 연결 된 델리게이트로 인해 공격 애니메이션 재생이 완료되면 호출된다
void AABCharacter::OnAttackMontageEneded(UAnimMontage * Montage, bool bInterrupted)
{
	// 공격 중이었는지 체크
	ABCHECK(IsAttacking);

	// 하나라도 실행했는지 체크
	ABCHECK(CurrentCombo > 0);

	// 공격 중 플래그 끈다
	IsAttacking = false;

	// 콤보가 끊겼으므로 콤보 관련 값들 초기화
	AttackEndComboState();
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;

	// CurrentCombo 값이 0 이상 MaxCombo - 1 이하인지 체크
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));

	// CurrentCombo + 1이 1과 MaxCombo 값 사이에 항상 존재하도록 보정한다
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}
