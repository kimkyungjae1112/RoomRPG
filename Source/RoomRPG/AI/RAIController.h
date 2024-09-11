// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API ARAIController : public AAIController
{
	GENERATED_BODY()

public:
	ARAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
