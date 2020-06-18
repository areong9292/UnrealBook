// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AABGameMode();
	
public:
	// 플레이어 로그인 후 게임모드는 PostLogin을 호출한다
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
