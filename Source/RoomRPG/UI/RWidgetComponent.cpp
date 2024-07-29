// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RWidgetComponent.h"
#include "UI/RUserWidget.h"

void URWidgetComponent::InitWidget()
{
	Super::InitWidget();

	URUserWidget* UserWidget = Cast<URUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetOwningActor(GetOwner());
	}
}
