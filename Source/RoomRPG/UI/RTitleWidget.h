// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API URTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URTitleWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess))
	TObjectPtr<class UButton> PlayButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess))
	TObjectPtr<class UButton> QuiteButton;

};
