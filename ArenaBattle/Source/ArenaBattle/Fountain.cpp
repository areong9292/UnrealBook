// Fill out your copyright notice in the Description page of Project Settings.

#include "Fountain.h"


// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component 생성
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));

	// 루트 컴포넌트로 Body 지정, Body밑에 Water를 자식으로 붙힘
	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	// F 접두사 - 언리얼 오브젝트와 관련 없는 일반 c++ 클래스, 구조체
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	// 에셋 경로로 에셋 부르기
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01"));

	// 부르기 성공했으면
	if (SM_BODY.Succeeded())
	{
		// SetStaticMesh에 에셋의 포인터(SM_BODY.Object)를 전달한다
		Body->SetStaticMesh(SM_BODY.Object);
	}

	// 에셋 경로로 에셋 부르기
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02"));

	// 부르기 성공했으면
	if (SM_WATER.Succeeded())
	{
		// SetStaticMesh에 에셋의 포인터(SM_BODY.Object)를 전달한다
		Water->SetStaticMesh(SM_WATER.Object);
	}

	// 에셋 경로로 에셋 부르기
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_SLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01"));

	// 부르기 성공했으면
	if (PS_SLASH.Succeeded())
	{
		// SetStaticMesh에 에셋의 포인터(SM_BODY.Object)를 전달한다
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

