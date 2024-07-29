// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RInteractionDoorUI.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API URInteractionDoorUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URInteractionDoorUI(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UTextBlock> TextButton;
};
