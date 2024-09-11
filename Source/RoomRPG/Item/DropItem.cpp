// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DropItem.h"
#include "Stat/RMonsterStat.h"

// Sets default values for this component's properties
UDropItem::UDropItem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDropItem::Drop()
{
	FTimerHandle DeadTimer;
	GetWorld()->GetTimerManager().SetTimer(DeadTimer, [this]()
		{
			GetWorld()->SpawnActor<AActor>(ItemClass, GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, -90.0f), FRotator::ZeroRotator);
		}, MonsStat->GetDestroyTime(), false);
}

void UDropItem::SetDropItem(URMonsterStat* InMonsStat)
{
	MonsStat = InMonsStat;
	MonsStat->OnHpZeroDelegate.AddUObject(this, &UDropItem::Drop);
}



