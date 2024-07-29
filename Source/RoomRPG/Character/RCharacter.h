// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Interface/RAnimationAttackInterface.h"
#include "Interface/RInteractionDoorInterface.h"
#include "Interface/RCharacterWidgetInterface.h"
#include "Interface/RWallInterface.h"
#include "RCharacter.generated.h"

UCLASS()
class ROOMRPG_API ARCharacter : public ACharacter, public IRAnimationAttackInterface, public IRInteractionDoorInterface, public IRCharacterWidgetInterface, public IRWallInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE uint8 GetKeyIndex() const { return KeyIndex; }
	FORCEINLINE void SetKeyIndex() { KeyIndex++; }

//Camera
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> Camera;


//Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputAction> R_SkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputAction> DoorAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<class UInputAction> ESCAction;


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run();
	void StopRuning();
	void Attack();
	void R_SkillBegin();
	void R_SkillEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void DisplayMenu();

//Combo Action
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URComboActionData> ComboActionData;

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;

	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

//Skill
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> R_SkillActionMontage;
	
	bool bSkillCooldown;
	float SkillCooldownTime;
	FTimerHandle SkillCooldownHandle;
	void ResetSkillCooldown();

//Attack Hit Check
private:
	virtual void AttackHitCheck() override;
	virtual void R_SkillHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


//Door Interaction
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	TSubclassOf<UUserWidget> DoorUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<UUserWidget> DoorUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<class ACellDoor> Door;

	void OpenDoor();

	virtual void DisplayDoorUI(AActor* InActor) override;
	virtual void UnDisplayDoorUI() override;

//UI
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<class URWidgetComponent> HpBar;

	virtual void SetHUD(class URHUDWidget* HUDWidget) override;

	bool bFlagESC = true;
//Stat
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<class URCharacterStat> Stat;

//Dead
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadActionMontage;
	
	void Dead();
	
//Key
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ARKey> Key;
	
	void GetKeyInputF();

	uint8 KeyIndex;

	virtual void WatchWall() override;
	virtual bool CheckKey() override;

//Spawn
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> StartMontage;

	void EndSpawn(class UAnimMontage* TargetMontage, bool bIsProperlyEnded);

};
