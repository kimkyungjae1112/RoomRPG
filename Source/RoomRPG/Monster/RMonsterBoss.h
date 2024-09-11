// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/RMonsterBase.h"
#include "Interface/RAnimationAttackInterface.h"
#include "Interface/RBossBombInterface.h"
#include "RMonsterBoss.generated.h"

/**
 * 
 */
UCLASS()
class ROOMRPG_API ARMonsterBoss : public ARMonsterBase, public IRAnimationAttackInterface, public IRBossBombInterface
{
	GENERATED_BODY()
	
public:
	ARMonsterBoss();

//AI Interface
	virtual void AttackByAI() override;
	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) override;
	virtual void BossSkill_2(APawn* Player) override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;

	virtual void TakeDamageByBomb(AActor* InOtherActor, float Damage) override;

//MonsterBase
	virtual void AttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) override;

protected:
	virtual void BeginPlay() override;

//AI 행동
private:
	void BeginAttack();
	void Dead();
	void BeginSkill_1(float InCurrentHp);
	void EndSkill_1(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void BeginSkill_2(APawn* Player);
	void EndSkill_2(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EvenetInstigator, AActor* DamageCauser) override;

private:
	//Animation
	FAIMonsterAttackFinished OnAttackFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> Skill1Montage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> Skill2Montage;

	//Animation Hit Check
	virtual void AttackHitCheck() override;
	virtual void R_SkillHitCheck() override;

//스킬
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess))
	TSubclassOf<class ARMonsterScary> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess))
	TSubclassOf<class ARBomb> Bomb;

//스텟
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess))
	TObjectPtr<class URBossStat> Stat;

//기믹
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick", meta = (AllowPrivateAccess))
	TSubclassOf<class ARBossRoom> BossRoomClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ARBossRoom> BossRoom;

	void BossStart();
};
