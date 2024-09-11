// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/RMonsterBase.h"
#include "Interface/RAnimationAttackInterface.h"
#include "RMonsterScary.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API ARMonsterScary : public ARMonsterBase, public IRAnimationAttackInterface
{
	GENERATED_BODY()

public:
	ARMonsterScary();

protected:
	virtual void BeginPlay() override;

public:
	virtual void AttackByAI() override;
	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) override;
	virtual void AttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) override;

	virtual void RunToCharacter() override;
	virtual void WalkToCharacter() override;
	virtual void BossSkill_2(APawn* Player) override;

//HitCheck
public:
	virtual void AttackHitCheck() override;
	virtual void R_SkillHitCheck() override;

//Animation
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> SkillMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> HitReactionMontage;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void RunSkill(float InCurrentHp);
	void SkillEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void RunAttack();
	void SetDead();
	void BeginHitReaction(float InCurrentHp);
	void EndHitReaction(class UAnimMontage* TargetMontage, bool IsProperlyEnded);


	FAIMonsterAttackFinished OnAttackFinished;

private:
	UPROPERTY(EditInstanceOnly)
	float MaxSpeed;

	UPROPERTY(EditInstanceOnly)
	float CurrentSpeed;

	bool bFlag = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat", meta=(AllowPrivateAccess))
	TObjectPtr<class URMonsterStat> Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess))
	TObjectPtr<class UDropItem> DropItem;
};
