// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "RAI.h"
#include "Interface/RMonsterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type bResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IRMonsterAIInterface* AIPawn = Cast<IRMonsterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAIMonsterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();

	return EBTNodeResult::InProgress;
}
