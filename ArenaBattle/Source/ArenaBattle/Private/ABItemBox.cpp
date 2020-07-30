// Fill out your copyright notice in the Description page of Project Settings.

#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 캐릭터와의 충돌을 감지할 콜리전 박스
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));

	// 박스 스태틱 메시(눈에 보이는 리소스)
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	// 콜리전 박스 컴포넌트를 루트로 지정
	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	// SetBoxExtent - 전체 박스 영역의 절반
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	// 리소스 불러온다
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	// 리소스 불러온다
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// 박스컴포넌트만 콜리전 적용
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	// 무기 클래스 정보 저장
	WeaponItemClass = AABWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 콜리전 겹침에는 Overlap 이벤트가 발생하고
	// 박스 컴포넌트에는 Overlap을 처리할 수 있게 OnComponentBeginOverlap 가 있다

	// OnComponentBeginOverlap - 호출 때마다 OnCharacterOverlap 호출되게 델리게이트 연결
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABLOG_S(Warning);

	auto ABCharacter = Cast<AABCharacter>(OtherActor);
	ABCHECK(ABCharacter != nullptr);

	if (ABCharacter != nullptr && WeaponItemClass != nullptr)
	{
		// 무기 착용이 가능한 경우에만
		if (ABCharacter->CanSetWeapon())
		{
			// 액터를 월드에 스폰한 뒤 캐릭터에 장착시킨다
			auto NewWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			ABCharacter->SetWeapon(NewWeapon);

			// 무기 스폰 후 이펙트 재생
			Effect->Activate(true);
			
			// 충돌 관련 끄고
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);

			// 이펙트 종료 델리게이트 연결
			Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
		}
		else
		{
			ABLOG(Warning, TEXT("%s can't equip weapon currently"), *ABCharacter->GetName());
		}
	}
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Destroy();
}
