// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RInteractionDoorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URInteractionDoorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROOMRPG_API IRInteractionDoorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void DisplayDoorUI(class AActor* InActor) = 0;
	virtual void UnDisplayDoorUI() = 0;
};
