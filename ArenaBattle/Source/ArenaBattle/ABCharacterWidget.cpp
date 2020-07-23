// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterWidget.h"
#include "ABCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UABCharacterWidget::BindCharacterStat(class UABCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(NewCharacterStat != nullptr);

	CurrentCharacterStat = NewCharacterStat;

	// UI ������ �÷��̾� ��Ʈ�ѷ��� BeginPlay���� ȣ��ȴ�
	// �׷��Ƿ� BeginPlay ���� ȣ��� PostInitializeComponents������ ������ �ݿ����� �ʴ´�
	// �׷��� ���⼭�� �׳� UpdateHPWidget ���Ḹ �صΰ� NativeConstruct���� �ѹ� ���õǰ� �Ѵ�
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

// UI ������ �÷��̾� ��Ʈ�ѷ��� BeginPlay���� ȣ��ȴ�
// �׷��Ƿ� BeginPlay ���� ȣ��� PostInitializeComponents������ ������ �ݿ����� �ʴ´�
// NativeConstruct���� ���� �����ϴ� �� �߰��ؼ� ù ���¶��� ���α׷����ٰ� ���������� ����ǰ� �Ѵ�
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
