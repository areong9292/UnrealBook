// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameInstance.h"


UABGameInstance::UABGameInstance()
{
	// 테이블 데이터 레퍼런스
	FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");

	// 레퍼런스로 실 데이터를 불러온다
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);

	// 잘 불러왔는지 체크 후
	ABCHECK(DT_ABCHARACTER.Succeeded());

	// 테이블 데이터 변수 지정해서 값을 가지게 한다
	ABCharacterTable = DT_ABCHARACTER.Object;

	// RowMap - Map of name of row to row data structure
	// 구조체 정보 있는지 체크
	ABCHECK(ABCharacterTable->RowMap.Num() > 0);
}

// 게임 앱이 초기화되면 (맵&모드에 지정한)GameInstance의 Init함수가 호출된다
void UABGameInstance::Init()
{
	Super::Init();

	// 잘 셋팅됬는지 DropExp 값 임시로 땡겨온다
	ABLOG(Warning, TEXT("DropExp of Level 20 ABCharacter : %d"), GetABCharacterData(20)->DropExp);
}

// 키 값으로 테이블에 접근하여 값을 가져오는 메소드
FABCharacterData * UABGameInstance::GetABCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
}
