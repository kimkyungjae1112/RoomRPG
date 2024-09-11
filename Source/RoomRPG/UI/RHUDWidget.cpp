// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RHUDWidget.h"
#include "UI/RHpBarWidget.h"
#include "Interface/RCharacterWidgetInterface.h"

URHUDWidget::URHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<URHpBarWidget>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HpProgressBar);

	HpProgressBar->SetMaxHp(400.0f);

	IRCharacterWidgetInterface* Player = Cast<IRCharacterWidgetInterface>(GetOwningPlayerPawn());
	if (Player)
	{
		Player->SetHUD(this);
	}
	
}

void URHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpProgressBar->UpdateHpBar(NewCurrentHp);
}
