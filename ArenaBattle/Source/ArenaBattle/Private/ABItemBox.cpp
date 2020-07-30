// Fill out your copyright notice in the Description page of Project Settings.

#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ĳ���Ϳ��� �浹�� ������ �ݸ��� �ڽ�
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));

	// �ڽ� ����ƽ �޽�(���� ���̴� ���ҽ�)
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	// �ݸ��� �ڽ� ������Ʈ�� ��Ʈ�� ����
	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	// SetBoxExtent - ��ü �ڽ� ������ ����
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	// ���ҽ� �ҷ��´�
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	// ���ҽ� �ҷ��´�
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// �ڽ�������Ʈ�� �ݸ��� ����
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	// ���� Ŭ���� ���� ����
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

	// �ݸ��� ��ħ���� Overlap �̺�Ʈ�� �߻��ϰ�
	// �ڽ� ������Ʈ���� Overlap�� ó���� �� �ְ� OnComponentBeginOverlap �� �ִ�

	// OnComponentBeginOverlap - ȣ�� ������ OnCharacterOverlap ȣ��ǰ� ��������Ʈ ����
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABLOG_S(Warning);

	auto ABCharacter = Cast<AABCharacter>(OtherActor);
	ABCHECK(ABCharacter != nullptr);

	if (ABCharacter != nullptr && WeaponItemClass != nullptr)
	{
		// ���� ������ ������ ��쿡��
		if (ABCharacter->CanSetWeapon())
		{
			// ���͸� ���忡 ������ �� ĳ���Ϳ� ������Ų��
			auto NewWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			ABCharacter->SetWeapon(NewWeapon);

			// ���� ���� �� ����Ʈ ���
			Effect->Activate(true);
			
			// �浹 ���� ����
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);

			// ����Ʈ ���� ��������Ʈ ����
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
