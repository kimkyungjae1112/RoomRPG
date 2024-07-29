// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPlayerController.h"
#include "UI/RHUDWidget.h"
#include "GameFramework/Character.h"

ARPlayerController::ARPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<URHUDWidget> HUDWidgetRef(TEXT("/Game/RoomRPG/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetRef.Class)
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> DeathWidgetClassRef(TEXT("/Game/RoomRPG/UI/WBP_Death.WBP_Death_C"));
	if (DeathWidgetClassRef.Class)
	{
		DeathWidgetClass = DeathWidgetClassRef.Class;
	}
}

void ARPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	HUDWidget = CreateWidget<URHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

void ARPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUDWidget->RemoveFromViewport();

	if (!bIsWinner)
	{
		UUserWidget* DeathScreen = CreateWidget(this, DeathWidgetClass);
		if (DeathScreen)
		{
			DeathScreen->AddToViewport();
		}
	}
	FTimerHandle ReStartTimer;
	GetWorldTimerManager().SetTimer(ReStartTimer, this, &APlayerController::RestartLevel, 5.0f);

}

void ARPlayerController::GamePause()
{
	SetPause(false);
}


