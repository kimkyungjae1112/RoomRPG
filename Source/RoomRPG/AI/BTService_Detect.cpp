// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "RAI.h"
#include "AIController.h"
#include "Interface/RMonsterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (!World)
	{
		return;
	}

	IRMonsterAIInterface* AIPawn = Cast<IRMonsterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool HitResult = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParams);

	if (HitResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);

				/* 디버그 */
				/*DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 1.0f);
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 1.0f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 1.0f);*/

				//달려가기
				AIPawn->RunToCharacter();
				double Distance = FVector::Distance(ControllingPawn->GetActorLocation(), Pawn->GetActorLocation());
				//UE_LOG(LogTemp, Display, TEXT("Distance : %f"), Distance);
				if (FMath::RandRange(1.0, 10.0) <= 3.5 && Distance >= 300.0f)
				{
					AIPawn->BossSkill_2(Pawn);
				}
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 1.0f);

	AIPawn->WalkToCharacter();
}
