// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RTitleMenuPlayerController.h"

ARTitleMenuPlayerController::ARTitleMenuPlayerController()
{
}

void ARTitleMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
}

