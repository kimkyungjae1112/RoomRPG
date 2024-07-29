// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/RMonsterStat.h"

// Sets default values for this component's properties
URMonsterStat::URMonsterStat()
{
	MaxHp = 200.0f;
	DestroyTime = 3.0f;
	Damage = 15.0f;
}

void URMonsterStat::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}

float URMonsterStat::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZeroDelegate.Broadcast();
	}
	return ActualDamage;
}

void URMonsterStat::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	UE_LOG(LogTemp, Display, TEXT("현재 체력 : %f"), CurrentHp);
	OnHpChangedDelegate.Broadcast(CurrentHp);
}





