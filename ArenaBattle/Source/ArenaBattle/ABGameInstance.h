// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "ABGameInstance.generated.h"

/**
 * 
 */

// CSV 파일을 위한 구조체
// 각 열의 이름과 유형이 동일해야한다

// 테이블 용으로 제공되는 FTableRowBase를 상속받는 구조체를 선언한다
USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	// CSV 파일의 각 열의 이름과 동일한 멤버 변수를 타입에 맞춰 선언
	// 젤 처음인 Name 열은 언리얼 엔진에서 자동으로 키 값으로 사용하므로 선언 안함
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

// GameInstance - 게임 전체적으로...? 관리하는 클래스
UCLASS()
class ARENABATTLE_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UABGameInstance();
	
	virtual void Init() override;

	FABCharacterData* GetABCharacterData(int32 Level);

private:
	// 테이블 데이터 관리를 위해 DataTable이라는 언리얼 오브젝트 제공됨
	UPROPERTY()
		class UDataTable* ABCharacterTable;
};
