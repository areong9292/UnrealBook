// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterWidget.h"
#include "ABCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UABCharacterWidget::BindCharacterStat(class UABCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(NewCharacterStat != nullptr);

	CurrentCharacterStat = NewCharacterStat;

	// UI 생성은 플레이어 컨트롤러의 BeginPlay에서 호출된다
	// 그러므로 BeginPlay 전에 호출된 PostInitializeComponents에서의 로직은 반영되지 않는다
	// 그래서 여기서는 그냥 UpdateHPWidget 연결만 해두고 NativeConstruct에서 한번 셋팅되게 한다
	NewCharacterStat->OnHPChanged.AddUObject(this, &UABCharacterWidget::UpdateHPWidget);
	/*
	NewCharacterStat->OnHPChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat.IsValid())
		{
			ABLOG(Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
		}
	});
	*/
}

// UI 생성은 플레이어 컨트롤러의 BeginPlay에서 호출된다
// 그러므로 BeginPlay 전에 호출된 PostInitializeComponents에서의 로직은 반영되지 않는다
// NativeConstruct에서 값을 셋팅하는 걸 추가해서 첫 오픈때도 프로그래스바가 정상적으로 적용되게 한다
void UABCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ABCHECK(HPProgressBar != nullptr);
	UpdateHPWidget();
}

void UABCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (HPProgressBar != nullptr)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
