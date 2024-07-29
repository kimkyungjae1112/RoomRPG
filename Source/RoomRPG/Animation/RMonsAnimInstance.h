// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RMonsAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API URMonsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URMonsAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSecondes) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float JumpingThreshould;
};
