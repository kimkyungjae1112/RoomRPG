// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RMonsterBoss.h"
#include "Components/CapsuleComponent.h"
#include "Stat/RBossStat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AI/RAIController.h"
#include "Engine/DamageEvents.h"
#include "Monster/RMonsterScary.h"
#include "Prop/RBomb.h"
#include "Gimmick/RBossRoom.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"

ARMonsterBoss::ARMonsterBoss()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Attaked"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/RoomRPG/Boss/Maw_J_Laygo.Maw_J_Laygo'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}
	
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	GetCharacterMovement()->MaxWalkSpeed = 150.0f;


	//Animation
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/RoomRPG/Animation/Boss/AM_DefaultAttack.AM_DefaultAttack'"));
	if (DefaultAttackMontageRef.Object)
	{
		DefaultAttackMontage = DefaultAttackMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/RoomRPG/Animation/Boss/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill1MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/RoomRPG/Animation/Boss/AM_Skill_1.AM_Skill_1'"));
	if (Skill1MontageRef.Object)
	{
		Skill1Montage = Skill1MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill2MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/RoomRPG/Animation/Boss/AM_Skill_2.AM_Skill_2'"));
	if (Skill2MontageRef.Object)
	{
		Skill2Montage = Skill2MontageRef.Object;
	}
	
	//Stat
	Stat = CreateDefaultSubobject<URBossStat>(TEXT("Stat"));
	Stat->OnHpZeroDelegate.AddUObject(this, &ARMonsterBoss::Dead);
	Stat->OnHpChangedDelegate.AddUObject(this, &ARMonsterBoss::BeginSkill_1);

	//투사체
	static ConstructorHelpers::FClassFinder<ARBomb> BombRef(TEXT("/Game/RoomRPG/Blueprints/BP_Bomb.BP_Bomb_C"));
	if (BombRef.Class)
	{
		Bomb = BombRef.Class;
	}

	//기믹
	static ConstructorHelpers::FClassFinder<ARBossRoom> BossRoomClassRef(TEXT("/Game/RoomRPG/Blueprints/BP_BossRoom.BP_BossRoom_C"));
	if (BossRoomClassRef.Class)
	{
		BossRoomClass = BossRoomClassRef.Class;
	}
}

void ARMonsterBoss::AttackByAI()
{
	BeginAttack();
}

void ARMonsterBoss::SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ARMonsterBoss::BossSkill_2(APawn* Player)
{
	BeginSkill_2(Player);
}

float ARMonsterBoss::GetAIDetectRange()
{
	return 1500.0f;
}

float ARMonsterBoss::GetAIAttackRange()
{
	return 300.0f;
}

void ARMonsterBoss::TakeDamageByBomb(AActor* InOtherActor, float Damage)
{
	FDamageEvent DamageEvent;
	InOtherActor->TakeDamage(Damage, DamageEvent, GetController(), this);
}

void ARMonsterBoss::AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	Super::AttackEnd(TargetMontage, IsProperlyEnded);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

void ARMonsterBoss::BeginPlay()
{
	Super::BeginPlay();

	ARAIController* AIController = Cast<ARAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	for (TActorIterator<ARBossRoom> It(GetWorld()); It; ++It)
	{
		BossRoom = *It;
		break; 
	}

	if (BossRoom)
	{
		BossRoom->OnEnterBossRoom.BindUObject(this, &ARMonsterBoss::BossStart);
	}
}

void ARMonsterBoss::BeginAttack()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DefaultAttackMontage, 1.0f);
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARMonsterBoss::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DefaultAttackMontage);
}

void ARMonsterBoss::Dead()
{
	ARAIController* AIController = Cast<ARAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DeadMontage);
	}

	SetActorEnableCollision(false);

	if (BossRoom)
	{
		BossRoom->SetWinTrigger();
	}
}

void ARMonsterBoss::BeginSkill_1(float InCurrentHp)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if ((InCurrentHp / Stat->GetMaxHp() <= 0.80f) && FMath::RandRange(1.0, 10.0) <= 2.5)
		{
			ARAIController* AIController = Cast<ARAIController>(GetController());
			if (AIController)
			{
				SetActorEnableCollision(false);
				AIController->StopAI();
				AnimInstance->StopAllMontages(0.0f);
				AnimInstance->Montage_Play(Skill1Montage);
			}
		}
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARMonsterBoss::EndSkill_1);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Skill1Montage);
}

void ARMonsterBoss::EndSkill_1(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	FVector RandomPos(GetActorLocation().X + 100, GetActorLocation().Y + 100, GetActorLocation().Z);
	AActor* Actor = GetWorld()->SpawnActor<ARMonsterScary>(MonsterClass, RandomPos, GetActorRotation());


	ARAIController* AIController = Cast<ARAIController>(GetController());
	if (AIController)
	{
		AIController->RunAI();
		SetActorEnableCollision(true);
	}
}

void ARMonsterBoss::BeginSkill_2(APawn* Player)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		ARAIController* AIController = Cast<ARAIController>(GetController());
		if (AIController)
		{
			AIController->StopAI();

			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(Skill2Montage, 1.5f);

			FVector Location = GetMesh()->GetSocketLocation(TEXT("RightHand"));
			ARBomb* BombActor = GetWorld()->SpawnActor<ARBomb>(Bomb, Location, GetActorRotation());
			BombActor->SetDirection(Player->GetActorLocation());
			BombActor->SetOwner(this);
		}
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARMonsterBoss::EndSkill_2);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Skill2Montage);
}

void ARMonsterBoss::EndSkill_2(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ARAIController* AIController = Cast<ARAIController>(GetController());
	if (AIController)
	{
		AIController->RunAI();
	}
}

float ARMonsterBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EvenetInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EvenetInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void ARMonsterBoss::AttackHitCheck()
{
	const float AttackRange = 300.0f;
	const float AttackDamage = 30.0f;

	FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, -50.0f);
	FVector End = Start + AttackRange;

	FCollisionQueryParams Params(NAME_None, false, this);

	FHitResult HitResult;
	bool bResult = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeBox(FVector3f(300.0f, 100.0f, 100.0f)), Params);
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
	
	/* 디버그 */
	//FColor Color = bResult ? FColor::Green : FColor::Red;
	//DrawDebugBox(GetWorld(), Start, FVector(150.0f, 100.0f, 100.0f), Color, false, 3.0f);
}

void ARMonsterBoss::R_SkillHitCheck()
{
}

void ARMonsterBoss::BossStart()
{
	ARAIController* AIController = Cast<ARAIController>(GetController());
	if (AIController)
	{
		AIController->RunAI();
	}
}

