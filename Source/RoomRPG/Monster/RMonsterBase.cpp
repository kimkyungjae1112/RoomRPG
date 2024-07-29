// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RMonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "AI/RAIController.h"
#include "Stat/RMonsterStat.h"
#include "Animation/AnimMontage.h"
#include "Item/DropItem.h"
// Sets default values
ARMonsterBase::ARMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Attacked"));

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	AIControllerClass = ARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	
}

// Called when the game starts or when spawned
void ARMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ARMonsterBase::GetAIPatrolRadius()
{
	return 800.0f;
}

float ARMonsterBase::GetAIDetectRange()
{
	return 800.0f;
}

float ARMonsterBase::GetAIAttackRange()
{
	return 200.0f;
}

float ARMonsterBase::GetAITurnSpeed()
{
	return 2.0f;
}

void ARMonsterBase::AttackByAI()
{
}

void ARMonsterBase::SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished)
{
}

void ARMonsterBase::RunToCharacter()
{
}

void ARMonsterBase::WalkToCharacter()
{
}

void ARMonsterBase::BossSkill_2(APawn* Player)
{
}

void ARMonsterBase::AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
}


