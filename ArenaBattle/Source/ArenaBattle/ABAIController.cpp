// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPos(TEXT("PatrolPos"));

AABAIController::AABAIController()
{
	// �� ���� - AI �Ǵ��� ���� ���̴� ������ ����
	// ������ ����
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_ABCharacter.BB_ABCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	// �����̺�� Ʈ�� - ������ �����Ϳ� ��ݿ� ������ Ʈ��
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_ABCharacter.BT_ABCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

// ���� �����Ѵ�
void AABAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	// Blackboard - AIController�� �ִ� UBlackboardComponent
	// UseBlackboard - Blackboard�� BBAsset ����
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// �����忡 ���� ������ �� ��ġ ���� HomePosKey�� ����
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());

		// BBAsset�����ͷ� BTAsset ����
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}
