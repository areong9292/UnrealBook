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

	// Component 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	// 다른 컴포넌트들은 캡슐 밑에 붙힌다
	SpringArm->SetupAttachment(GetCapsuleComponent());

	// 카메라는 스프링 암 밑에 붙힌다
	Camera->SetupAttachment(SpringArm);

	// UI 메시에 붙힌다
	HPBarWidget->SetupAttachment(GetMesh());

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

	// 캐릭터 위에 위치하게 함
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	// 항상 플레이어를 바라보게 Screen으로 셋팅
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// UI 블루프린트 가져온다
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);

		// UI 사이즈와 동일한 크기로 셋팅
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
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

	// 콜리전 프리셋 셋팅
	// 캡슐 컴포넌트가 새로 만든 ABCharacter를 프리셋으로 사용하게 한다
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// 공격 범위, 반경을 지정한다
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	/*
	// 무기 붙히려 하는 소켓 이름 변수로 저장
	// 소켓은 스켈레탈 메시 컴포넌트의 스켈레탈 메시에 있다
	// GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object); 로직으로
	// 먼저 셋팅 후 소켓 있는지 체크해야 한다
	FName WeaponSocket(TEXT("hand_rSocket"));

	// 메시 안에 해당 소켓이 있는지 체크
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		// 무기 에셋 불러온다
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}

		// 무기를 메시에 붙히는데 인자로 소켓 이름(WeaponSocket)을 전달하면
		// 소켓 위치를 기준으로 트랜스 폼이 자동으로 설정된다
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	*/

	// ABAIController 인공지능 설정
	AIControllerClass = AABAIController::StaticClass();

	// 플레이어가 조종하는 캐릭터를 제외한 모든
	// 앞으로 레벨에 배치하거나 새롭게 생성되는 캐릭터는 자동으로 ABAIController를 가진다
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

	// 액터를 월드에 스폰한다
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon != nullptr)
	{
		// 그 후 스폰한 액터를 캐릭터의 WeaponSocket을 찾아서 거기에 붙힌다
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
		// ABLOG(Warning, TEXT("OnNextAttackCheck"));
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

	// 애님 인스턴스의 OnAttackHitCheck 델리게이트에 AttackCheck 로직 등록
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	// 델리게이트 선언
	// 해당 델리게이트가 호출되면 죽음 처리한다
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

	// IE_Pressed - 눌렸을 경우 호출
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);

	// 상속받은 ACharacter에 점프 기능이 이미 있다
	// 바인딩만 해주면 작동함
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	
	// IE_Pressed - 눌렸을 경우 호출
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

// 공격을 받으면 호출된다
float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// 부모 클래스인 AActor에 기본적인 데미지 관련 로직이 구현되어 있다
	// 결과로 받은 데미지를 얻는다
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	/*
	// 데미지가 들어왔을 경우
	if (FinalDamage > 0.0f)
	{
		// 죽는 애니메이션 재생을 위해 애님 인스턴스(애니메이션 블루프린트)의
		// IsDead 플래그를 킨다
		ABAnim->SetDeadAnim();

		// 액터 콜리전을 꺼버려서 충돌 이벤트가 더 이상 발생되지 않게 한다
		SetActorEnableCollision(false);
	}
	*/

	// 데미지 처리를 스텟 컴포넌트에서 한다
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
	// NPC의 경우 회전을 자연스럽게 별도 셋팅한 뒤 속도를 줄인다
	else
	{
		ABLOG(Warning, TEXT("5678Actor : %s"), *GetName());
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

// 무기 착용 가능한지 체크
bool AABCharacter::CanSetWeapon()
{
	return (CurrentWeapon == nullptr);
}

// 무기 착용
void AABCharacter::SetWeapon(AABWeapon * NewWeapon)
{
	ABCHECK(NewWeapon != nullptr && CurrentWeapon == nullptr);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if(NewWeapon != nullptr)
	{
		// 무기를 스폰하고 캐릭터가 이 무기를 소유하게 한다
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

	// 애니메이션 재생 완료 시 델리게이트 실행
	OnAttackEnd.Broadcast();
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

void AABCharacter::AttackCheck()
{
	// 공격 범위에 액터가 탐지될 경우 정보를 담게 될 구조체
	// HitResult의 Actor는 약 포인터(TWeakObjectPtr) 방식으로 선언되어있다
	// TWeakObjectPtr로 지정된 액터에 접근하려면 IsValid로 사용하려는 액터가 유효한지 먼저 점검하여야 한다
	// 언리얼에서 스마트 포인터 선언한거
	FHitResult HitResult;

	FCollisionQueryParams Params(NAME_None, false, this);

	// SweepSingleByChannel - 시작점부터 끝점까지 훑으면서 충돌을 감지한다
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),											// 시작점
		GetActorLocation() + GetActorForwardVector() * AttackRange,	// 끝점
		FQuat::Identity,											// 탐색에 쓸 도형의 회전
		ECollisionChannel::ECC_GameTraceChannel2,					// 탐색 채널
		FCollisionShape::MakeSphere(AttackRadius),					// 탐색에 쓸 도형정보
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;			// 캐릭터 시선 방향 벡터
	FVector Center = GetActorLocation() + TraceVec * 0.5f;				// 중간 지점
	float HalfHeight = AttackRange * 0.5f + AttackRadius;				// 중간 높이
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();	// 캡슐 Z를 캐릭터 시선 방향으로 향하게 회전
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;			// 공격 성공 여부에 따라 컬러 지정
	float DebugLifeTime = 5.0f;											// 5초 뒤 삭제

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

	// 충돌이 감지되었고
	if (bResult)
	{
		// 액터 정보가 있으면
		if (HitResult.Actor.IsValid())
		{
			// 충돌된 액터 이름을 로그찍는다
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			// AActor에는 TakeDamage가 구현되어있다
			// 공격 받는 액터에 TakeDamage를 override하고
			// 여기서 호출하면 공격받는 로직을 처리할 수 있다

			// 액터에 데미지를 전달한다
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(
				CharacterStat->GetAttack(),			// 전달할 데미지의 세기 - 데이터에서 불러온 값으로 셋팅한다
				DamageEvent,						// 데미지의 종류 - 여기서는 기본값
				GetController(),					// 공격 명령을 내린 가해자 - 데미지를 전달하는 주체는 폰을 조정하는 플레이어 컨트롤러다!
				this);								// 데미지 전달을 위해 사용한 도구 - 데미지 전달을 위해 ABCharacter라는 캐릭터(폰)을 사용한다!
		}
	}
}
