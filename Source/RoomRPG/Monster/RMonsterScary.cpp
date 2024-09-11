// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RMonsterScary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Stat/RMonsterStat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "AI/RAIController.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Item/DropItem.h"

ARMonsterScary::ARMonsterScary()
{
	//Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Scary_Zombie_Pack/Parasite_L_Starkie.Parasite_L_Starkie'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/RoomRPG/Animation/MonsAnim/AM_MonsAttack.AM_MonsAttack'"));
	if (AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SkillMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/RoomRPG/Animation/MonsAnim/AM_MonsSkill.AM_MonsSkill'"));
	if (SkillMontageRef.Object)
	{
		SkillMontage = SkillMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/RoomRPG/Animation/MonsAnim/AM_MonsDead.AM_MonsDead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/RoomRPG/Animation/MonsAnim/AM_HitReaction.AM_HitReaction'"));
	if (HitReactionMontageRef.Object)
	{
		HitReactionMontage = HitReactionMontageRef.Object;
	}

	//Stat
	Stat = CreateDefaultSubobject<URMonsterStat>(TEXT("Stat"));
	DropItem = CreateDefaultSubobject<UDropItem>(TEXT("DropItem"));

	Stat->OnHpZeroDelegate.AddUObject(this, &ARMonsterScary::SetDead);
	Stat->OnHpChangedDelegate.AddUObject(this, &ARMonsterScary::BeginHitReaction);
	Stat->OnHpChangedDelegate.AddUObject(this, &ARMonsterScary::RunSkill);


	MaxSpeed = 400.0f;
	CurrentSpeed = 100.0f;
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ARMonsterScary::BeginPlay()
{
	Super::BeginPlay();

	DropItem->SetDropItem(Stat);
}

void ARMonsterScary::AttackByAI()
{
	RunAttack();
}

void ARMonsterScary::SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ARMonsterScary::RunToCharacter()
{
	Super::RunToCharacter();

	CurrentSpeed = MaxSpeed;
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

void ARMonsterScary::WalkToCharacter()
{
	Super::WalkToCharacter();

	CurrentSpeed = 100.0f;
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

void ARMonsterScary::BossSkill_2(APawn* Player)
{
}

void ARMonsterScary::AttackHitCheck()
{
	const float AttackRange = 200.0f;
	const float Radius = 50.0f;

	float AttackDamage = Stat->GetDamage();

	FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 10.0f);
	FVector End = Start + AttackRange;

	FCollisionQueryParams Params(NAME_None, false, this);

	FHitResult HitResult;
	bool Result = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius), Params);

	if (Result)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	/* 디버그 */
	//FColor Color = Result ? FColor::Green : FColor::Red;
	//DrawDebugCapsule(GetWorld(), HitResult.ImpactPoint, 25.0f, Radius, FQuat::Identity, Color, false, 5.0f);
}

void ARMonsterScary::R_SkillHitCheck()
{
}

float ARMonsterScary::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void ARMonsterScary::RunSkill(float InCurrentHp)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && bFlag)
	{
		if ((InCurrentHp / Stat->GetMaxHp()) <= 0.6f)
		{
			SetActorEnableCollision(false);
			ARAIController* AIController = Cast<ARAIController>(GetController());
			if (AIController)
			{
				AIController->StopAI();
			}

			UE_LOG(LogTemp, Display, TEXT("스킬 발동!"));

			Stat->SetDamage(25.0f);
			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(SkillMontage, 1.3f);
		}
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARMonsterScary::SkillEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillMontage);
}

void ARMonsterScary::SkillEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	SetActorEnableCollision(true);

	ARAIController* AIController = Cast<ARAIController>(GetController());
	if (AIController)
	{
		AIController->RunAI();
	}

	bFlag = false;
}

void ARMonsterScary::RunAttack()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage, 1.5f);
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARMonsterScary::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void ARMonsterScary::AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	Super::AttackEnd(TargetMontage, IsProperlyEnded);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

void ARMonsterScary::SetDead()
{
	ARAIController* AIController = Cast<ARAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
		AIController->UnPossess();
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}	


	FTimerHandle DeadTimer;
	GetWorld()->GetTimerManager().SetTimer(DeadTimer, [this]()
		{
			Destroy();
		}, Stat->GetDestroyTime(), false);
}

void ARMonsterScary::BeginHitReaction(float InCurrentHp)
{
	if (InCurrentHp < 0.0f) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (AnimInstance->Montage_IsPlaying(SkillMontage)) return;
		ARAIController* AIController = Cast<ARAIController>(GetController());
		if (AIController)
		{
			AIController->StopAI();

			UE_LOG(LogTemp, Display, TEXT("HitReaction"));
			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(HitReactionMontage, 3.5f);
		}
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARMonsterScary::EndHitReaction);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, HitReactionMontage);
}

void ARMonsterScary::EndHitReaction(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ARAIController* AIController = Cast<ARAIController>(GetController());
	if (AIController)
	{
		AIController->RunAI();
	}
}
