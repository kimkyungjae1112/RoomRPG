// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/RMonsterAIInterface.h"
#include "RMonsterBase.generated.h"

UCLASS()
class ROOMRPG_API ARMonsterBase : public ACharacter, public IRMonsterAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//AI Interface
public:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void AttackByAI() override;
	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) override;

	virtual void RunToCharacter() override;
	virtual void WalkToCharacter() override;
	virtual void BossSkill_2(APawn* Player) override;


	//공격 끝나는 판정 함수
	virtual void AttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

//Damage
public:
	
};
