// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RMonsterStat.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /* CurrentHp */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMRPG_API URMonsterStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URMonsterStat();

protected:
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZeroDelegate;
	FOnHpChangedDelegate OnHpChangedDelegate;

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetDestroyTime() { return DestroyTime; }
	FORCEINLINE float GetDamage() { return Damage; }
	FORCEINLINE void SetDamage(float InDamage) { Damage = InDamage; }
	float ApplyDamage(float InDamage);


protected:
	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float MaxHp;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float CurrentHp;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float DestroyTime;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float Damage;
};
