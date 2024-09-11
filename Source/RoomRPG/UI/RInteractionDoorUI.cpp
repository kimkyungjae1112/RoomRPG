// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RInteractionDoorUI.h"
#include "Components/TextBlock.h"

URInteractionDoorUI::URInteractionDoorUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URInteractionDoorUI::NativeConstruct()
{
	Super::NativeConstruct();

	TextButton = Cast<UTextBlock>(GetWidgetFromName(TEXT("Button")));
}
