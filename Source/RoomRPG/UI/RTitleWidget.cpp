// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RTitleWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

URTitleWidget::URTitleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayButton = Cast<UButton>(GetWidgetFromName(TEXT("Play")));
	QuiteButton = Cast<UButton>(GetWidgetFromName(TEXT("Quite")));

    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &URTitleWidget::OnPlayButtonClicked);
    }

    if (QuiteButton)
    {
        QuiteButton->OnClicked.AddDynamic(this, &URTitleWidget::OnQuitButtonClicked);
    }

}

void URTitleWidget::OnPlayButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("Dungeon"));
}

void URTitleWidget::OnQuitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}