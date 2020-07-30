// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "ABGameInstance.generated.h"

/**
 * 
 */

// CSV ������ ���� ����ü
// �� ���� �̸��� ������ �����ؾ��Ѵ�

// ���̺� ������ �����Ǵ� FTableRowBase�� ��ӹ޴� ����ü�� �����Ѵ�
USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	// CSV ������ �� ���� �̸��� ������ ��� ������ Ÿ�Կ� ���� ����
	// �� ó���� Name ���� �𸮾� �������� �ڵ����� Ű ������ ����ϹǷ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int32 NextExp;
};

// GameInstance - ���� ��ü������...? �����ϴ� Ŭ����
UCLASS()
class ARENABATTLE_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UABGameInstance();
	
	virtual void Init() override;

	FABCharacterData* GetABCharacterData(int32 Level);

private:
	// ���̺� ������ ������ ���� DataTable�̶�� �𸮾� ������Ʈ ������
	UPROPERTY()
		class UDataTable* ABCharacterTable;
};
