// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/RMonsAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URMonsAnimInstance::URMonsAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
	RootMotionMode = ERootMotionMode::RootMotionFromEverything;

}

void URMonsAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	RootMotionMode = ERootMotionMode::RootMotionFromEverything;

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void URMonsAnimInstance::NativeUpdateAnimation(float DeltaSecondes)
{
	Super::NativeUpdateAnimation(DeltaSecondes);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
