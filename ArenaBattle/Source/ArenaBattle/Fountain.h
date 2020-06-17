// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"

// ==자동화 매크로== 클래스 분석 시 필요 정보 헤더
#include "Fountain.generated.h"

// ==클래스 선언 매크로== UCLASS(), GENERATED_BODY()
// ==클래스 선언 접두사== AFountain, Actor면 A, 아니면 U
// ==외부 모듈에의 공개여부== ARENABATTLE_API, 있으면 접근가능
UCLASS()
class ARENABATTLE_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
	// UPROPERTY() - 언리얼 엔진에서 자동으로 메모리 관리해준다
	// 단, 언리얼 객체만 가능하고 새로만든 일반 객체도 자동화해주려면
	// 특정 매크로를 선언해주어야한다 - ==자동화 매크로==
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Water;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent *Light;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *Splash;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;

private:
	// AllowPrivateAccess가 true이면 EditAnywhere이기에 에디터에서 수정은 가능하나
	// 코드 상에서는 private라 접근이 안된다
	// AllowPrivateAccess가 없으면 private, EditAnywhere 충돌나서 에러남
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float RotateSpeed;
};