// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameInstance.h"


UABGameInstance::UABGameInstance()
{
	// ���̺� ������ ���۷���
	FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");

	// ���۷����� �� �����͸� �ҷ��´�
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);

	// �� �ҷ��Դ��� üũ ��
	ABCHECK(DT_ABCHARACTER.Succeeded());

	// ���̺� ������ ���� �����ؼ� ���� ������ �Ѵ�
	ABCharacterTable = DT_ABCHARACTER.Object;

	// RowMap - Map of name of row to row data structure
	// ����ü ���� �ִ��� üũ
	ABCHECK(ABCharacterTable->RowMap.Num() > 0);
}

// ���� ���� �ʱ�ȭ�Ǹ� (��&��忡 ������)GameInstance�� Init�Լ��� ȣ��ȴ�
void UABGameInstance::Init()
{
	Super::Init();

	// �� ���É���� DropExp �� �ӽ÷� ���ܿ´�
	ABLOG(Warning, TEXT("DropExp of Level 20 ABCharacter : %d"), GetABCharacterData(20)->DropExp);
}

// Ű ������ ���̺� �����Ͽ� ���� �������� �޼ҵ�
FABCharacterData * UABGameInstance::GetABCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
}
