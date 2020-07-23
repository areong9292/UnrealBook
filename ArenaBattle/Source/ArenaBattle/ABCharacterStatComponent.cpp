// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// InitializeComponent 호출하려면 true로 해줘야한다
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UABCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(ABGameInstance != nullptr);
	CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);
	if (CurrentStatData != nullptr)
	{
		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP;
	}
	else
	{
		ABLOG(Error, TEXT("Level {%d} data doesn't exist"), NewLevel);
	}
}

// 현재 체력에서 받은 데미지를 감소시킨 후
// 결과가 0보다 작거나 같으면 델리게이트 작동시킨다
void UABCharacterStatComponent::SetDamage(float NewDamage)
{
	ABCHECK(CurrentStatData != nullptr)
	{
		CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
		if (CurrentHP <= 0.0f)
		{
			OnHPIsZero.Broadcast();
		}
	}
}

// 데미지 수치 리턴하는 메소드
float UABCharacterStatComponent::GetAttack()
{
	ABCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Attack;
}
