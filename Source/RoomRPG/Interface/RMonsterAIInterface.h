// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RMonsterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};


DECLARE_DELEGATE(FAIMonsterAttackFinished);
/**
 * 
 */
class ROOMRPG_API IRMonsterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0;

	virtual void RunToCharacter() = 0;
	virtual void WalkToCharacter() = 0;

	virtual void BossSkill_2(APawn* Player) = 0;
};