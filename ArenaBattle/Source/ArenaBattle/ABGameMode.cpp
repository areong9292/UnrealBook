// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// ����Ʈ ���� ABPawn���� �Ѵ�
	// AABPawn - ���� ���͸� ��ӹ޴´� �׷��� ���ξ� A ����
	// StaticClass() �Լ��� �̿��ϸ� ��Ÿ�� �߿� �ش� Ŭ������ ��Ÿ���� UClass�� �� �� �ִ�
	DefaultPawnClass = AABCharacter::StaticClass();

	// �÷��̾� ��Ʈ�ѷ��� ABPlayerController�� �Ѵ�
	PlayerControllerClass = AABPlayerController::StaticClass();
}

void AABGameMode::PostLogin(APlayerController * NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}
