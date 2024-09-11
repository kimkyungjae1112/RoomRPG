// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/RCharacterStat.h"

// Sets default values for this component's properties
URCharacterStat::URCharacterStat()
{
	MaxHp = 400.0f;
	CurrentHp = MaxHp;
	AttackSpeed = 1.0f;
	AttackDamage = 1.1f;
	WalkSpeed = 1.0f;
}

float URCharacterStat::ApplyDamage(float InDamage)
{
	const float PrevCurrentHp = CurrentHp;
	const float ActualDamage = InDamage;

	SetHp(CurrentHp - InDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnZero.Broadcast();
	}
	return ActualDamage;
}

void URCharacterStat::SetHp(float NewHp)
{
	UE_LOG(LogTemp, Display, TEXT("캐릭터 체력 : %f"), NewHp);
	CurrentHp = FMath::Clamp(NewHp, 0, MaxHp);
	OnChange.Broadcast(CurrentHp);
}


