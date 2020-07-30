// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UABCharacterStatComponent* NewCharacterStat);

protected:
	// UI 시스템이 준비되면 NativeConstruct가 호출된다
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
