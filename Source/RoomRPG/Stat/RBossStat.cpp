// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/RBossStat.h"

// Sets default values for this component's properties
URBossStat::URBossStat()
{
	MaxHp = 700.0f;
}


// Called when the game starts
void URBossStat::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}

float URBossStat::ApplyDamage(float InDamage)
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

void URBossStat::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	UE_LOG(LogTemp, Display, TEXT("현재 체력 : %f"), CurrentHp);
	OnHpChangedDelegate.Broadcast(CurrentHp);
}



