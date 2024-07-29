// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API URHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	

public:
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class URHpBarWidget> HpProgressBar;

};
