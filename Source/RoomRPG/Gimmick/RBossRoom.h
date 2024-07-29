// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RBossRoom.generated.h"

DECLARE_DELEGATE(FOnEnterBossRoom)

UCLASS()
class ROOMRPG_API ARBossRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARBossRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FOnEnterBossRoom OnEnterBossRoom;

	UFUNCTION()
	void OnBossRoomEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnWinRoomOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void SetWinTrigger();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enter", meta = (AllowPrivateAccess))
	TObjectPtr<class UBoxComponent> EnterTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enter", meta = (AllowPrivateAccess))
	TObjectPtr<class UBoxComponent> WinTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enter", meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> DoorWay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enter", meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> DoorRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enter", meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> DoorLeft;


};
