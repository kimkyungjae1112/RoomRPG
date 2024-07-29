// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DropItem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMRPG_API UDropItem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDropItem();

public:
	void Drop();

	void SetDropItem(class URMonsterStat* InMonsStat);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess))
	TSubclassOf<class ARKey> ItemClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess))
	TObjectPtr<class URMonsterStat> MonsStat;
		
};
