// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"

// ==�ڵ�ȭ ��ũ��== Ŭ���� �м� �� �ʿ� ���� ���
#include "Fountain.generated.h"

// ==Ŭ���� ���� ��ũ��== UCLASS(), GENERATED_BODY()
// ==Ŭ���� ���� ���λ�== AFountain, Actor�� A, �ƴϸ� U
// ==�ܺ� ��⿡�� ��������== ARENABATTLE_API, ������ ���ٰ���
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
	
	// UPROPERTY() - �𸮾� �������� �ڵ����� �޸� �������ش�
	// ��, �𸮾� ��ü�� �����ϰ� ���θ��� �Ϲ� ��ü�� �ڵ�ȭ���ַ���
	// Ư�� ��ũ�θ� �������־���Ѵ� - ==�ڵ�ȭ ��ũ��==
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
	// AllowPrivateAccess�� true�̸� EditAnywhere�̱⿡ �����Ϳ��� ������ �����ϳ�
	// �ڵ� �󿡼��� private�� ������ �ȵȴ�
	// AllowPrivateAccess�� ������ private, EditAnywhere �浹���� ������
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float RotateSpeed;
};