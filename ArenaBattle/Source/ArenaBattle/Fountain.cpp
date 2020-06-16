// Fill out your copyright notice in the Description page of Project Settings.

#include "Fountain.h"


// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component ����
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));

	// ��Ʈ ������Ʈ�� Body ����, Body�ؿ� Water�� �ڽ����� ����
	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	// F ���λ� - �𸮾� ������Ʈ�� ���� ���� �Ϲ� c++ Ŭ����, ����ü
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	// ���� ��η� ���� �θ���
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01"));

	// �θ��� ����������
	if (SM_BODY.Succeeded())
	{
		// SetStaticMesh�� ������ ������(SM_BODY.Object)�� �����Ѵ�
		Body->SetStaticMesh(SM_BODY.Object);
	}

	// ���� ��η� ���� �θ���
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02"));

	// �θ��� ����������
	if (SM_WATER.Succeeded())
	{
		// SetStaticMesh�� ������ ������(SM_BODY.Object)�� �����Ѵ�
		Water->SetStaticMesh(SM_WATER.Object);
	}

	// ���� ��η� ���� �θ���
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_SLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01"));

	// �θ��� ����������
	if (PS_SLASH.Succeeded())
	{
		// SetStaticMesh�� ������ ������(SM_BODY.Object)�� �����Ѵ�
		Splash->SetTemplate(PS_SLASH.Object);
	}
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

