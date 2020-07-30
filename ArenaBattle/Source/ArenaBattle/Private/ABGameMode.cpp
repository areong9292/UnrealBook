// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// 디폴트 폰을 ABPawn으로 한다
	// AABPawn - 폰은 액터를 상속받는다 그래서 접두어 A 붙음
	// StaticClass() 함수를 이용하면 런타임 중에 해당 클래스를 나타내는 UClass를 얻어낼 수 있다
	DefaultPawnClass = AABCharacter::StaticClass();

	// 플레이어 컨트롤러를 ABPlayerController로 한다
	PlayerControllerClass = AABPlayerController::StaticClass();
}

void AABGameMode::PostLogin(APlayerController * NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}
