// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API URAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSecondes) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float GroundSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")	
	uint8 bIsFalling : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float JumpingThreshould;
};
