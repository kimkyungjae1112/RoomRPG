// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RCharacterStat.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnZeroDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeDelegate, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMRPG_API URCharacterStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URCharacterStat();

public:
	FOnChangeDelegate OnChange;
	FOnZeroDelegate OnZero;

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetAttackSpeed() { return AttackSpeed; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }

	float ApplyDamage(float InDamage);
	void SetHp(float NewHp);

private:
	float MaxHp;
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat", meta = (AllowPrivateAccess))
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess))
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess))
	float WalkSpeed;
};
