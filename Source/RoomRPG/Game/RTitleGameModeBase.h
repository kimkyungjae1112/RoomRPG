// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RTitleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API ARTitleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARTitleGameModeBase();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Title", meta = (AllowPrivateAccess))
	TSubclassOf<class URTitleWidget> TitleWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Title", meta = (AllowPrivateAccess))
	TObjectPtr<class URTitleWidget> TitleWidget;

};
