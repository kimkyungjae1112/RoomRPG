// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RTitleGameModeBase.h"
#include "UI/RTitleWidget.h"
#include "GameFramework/PlayerController.h"

ARTitleGameModeBase::ARTitleGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Script/CoreUObject.Class'/Script/RoomRPG.RTitleMenuPlayerController'"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}

	static ConstructorHelpers::FClassFinder<URTitleWidget> TitleWidgetClassRef(TEXT("/Game/RoomRPG/UI/WBP_Title.WBP_Title_C"));
	if (TitleWidgetClassRef.Class)
	{
		TitleWidgetClass = TitleWidgetClassRef.Class;
	}
}

void ARTitleGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TitleWidget = CreateWidget<URTitleWidget>(GetWorld(), TitleWidgetClass);
	if (TitleWidget)
	{
		TitleWidget->AddToViewport();
	}
}
