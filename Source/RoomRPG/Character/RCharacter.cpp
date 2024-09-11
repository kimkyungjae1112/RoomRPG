// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "RComboActionData.h"
#include "Engine/DamageEvents.h"
#include "Prop/CellDoor.h"
#include "Item/RKey.h"
#include "UI/RInteractionDoorUI.h"
#include "UI/RWidgetComponent.h"
#include "UI/RHUDWidget.h"
#include "Stat/RCharacterStat.h"
#include "Player/RPlayerController.h"


ARCharacter::ARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RCapsule"));

	//Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 400.0f;

	//Movement
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollsion"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangAlbino.KwangAlbino'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/RoomRPG/Animation/ABP_RCharacter.ABP_RCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	//Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/RoomRPG/Input/IMC_Default.IMC_Default'"));
	if (InputMappingContextRef.Object)
	{
		InputMappingContext = InputMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/RoomRPG/Input/InputAction/IA_Move.IA_Move'"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/RoomRPG/Input/InputAction/IA_Look.IA_Look'"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/RoomRPG/Input/InputAction/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/RoomRPG/Input/InputAction/IA_Run.IA_Run'"));
	if (RunActionRef.Object)
	{
		RunAction = RunActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/RoomRPG/Input/InputAction/IA_Attack.IA_Attack'"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> R_SkillActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/RoomRPG/Input/InputAction/IA_R_Skill.IA_R_Skill'"));
	if (R_SkillActionRef.Object)
	{
		R_SkillAction = R_SkillActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> DoorActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/RoomRPG/Input/InputAction/IA_Door.IA_Door'"));
	if (DoorActionRef.Object)
	{
		DoorAction = DoorActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ESCActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/RoomRPG/Input/InputAction/IA_Pause.IA_Pause'"));
	if (ESCActionRef.Object)
	{
		ESCAction = ESCActionRef.Object;
	}
	
	Stat = CreateDefaultSubobject<URCharacterStat>(TEXT("Stat"));
	Stat->OnZero.AddUObject(this, &ARCharacter::Dead);

	KeyIndex = 0;

	//Skill Cooldown
	bSkillCooldown = false;
	SkillCooldownTime = 3.0f;

}

void ARCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	DoorUI = CreateWidget(GetWorld(), DoorUIClass);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PlayerController)
	{
		AnimInstance->Montage_Play(StartMontage, 1.0f);
		DisableInput(PlayerController);
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARCharacter::EndSpawn);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, StartMontage);
}

void ARCharacter::EndSpawn(UAnimMontage* TargetMontage, bool bIsProperlyEnded)
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	EnableInput(PlayerController);
}

void ARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARCharacter::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ARCharacter::Run);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ARCharacter::StopRuning);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ARCharacter::Attack);
	EnhancedInputComponent->BindAction(R_SkillAction, ETriggerEvent::Triggered, this, &ARCharacter::R_SkillBegin);
	EnhancedInputComponent->BindAction(DoorAction, ETriggerEvent::Started, this, &ARCharacter::OpenDoor);
	EnhancedInputComponent->BindAction(DoorAction, ETriggerEvent::Started, this, &ARCharacter::GetKeyInputF);
	EnhancedInputComponent->BindAction(ESCAction, ETriggerEvent::Started, this, &ARCharacter::DisplayMenu);
}

void ARCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputValue.X);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ARCharacter::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-InputValue.Y);
}

void ARCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void ARCharacter::StopRuning()
{
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
}

void ARCharacter::Attack()
{
	if (CurrentCombo == 0 && !GetCharacterMovement()->IsFalling())
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void ARCharacter::Dead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

	ARPlayerController* PlayerController = Cast<ARPlayerController>(GetController());
	ensure(PlayerController);
	DisableInput(PlayerController);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadActionMontage, 1.0f);

	PlayerController->GameHasEnded(this, false);
}

void ARCharacter::GetKeyInputF()
{
	if (DoorUI->IsInViewport())
	{
		if (Key)
		{
			Key->Destroy();
			SetKeyIndex();
		}
	}
}

void ARCharacter::WatchWall()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	FTimerHandle Temp;
	GetWorld()->GetTimerManager().SetTimer(Temp,
		[this]()
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}, 4.0f, false);
}

bool ARCharacter::CheckKey()
{
	return KeyIndex >= 5;
}

void ARCharacter::R_SkillBegin()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	if (bSkillCooldown)
	{
		UE_LOG(LogTemp, Display, TEXT("쿨타임!!"));
		return;
	}
	
	bSkillCooldown = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(R_SkillActionMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARCharacter::R_SkillEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, R_SkillActionMontage);
}

void ARCharacter::R_SkillEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	GetWorld()->GetTimerManager().SetTimer(SkillCooldownHandle, this, &ARCharacter::ResetSkillCooldown, SkillCooldownTime, false);
}

void ARCharacter::ResetSkillCooldown()
{
	UE_LOG(LogTemp, Display, TEXT("쿨타임 끝!!"));
	bSkillCooldown = false;
}

void ARCharacter::DisplayMenu()
{
	/*ARPlayerController* PlayerController = Cast<ARPlayerController>(GetController());
	if (PlayerController && bFlagESC)
	{
		PlayerController->Pause();
		bFlagESC = !bFlagESC;
	}
	else if (PlayerController && !bFlagESC)
	{
		PlayerController->GamePause();
		bFlagESC = !bFlagESC;
	}*/
}

void ARCharacter::ComboActionBegin()
{
	CurrentCombo = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate * Stat->GetAttackSpeed());

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ARCharacter::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ARCharacter::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ARCharacter::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ARCharacter::ComboCheck, ComboEffectiveTime, false);
	}
}

void ARCharacter::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void ARCharacter::AttackHitCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	
	const float AttackRange = 50.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool Hit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (Hit)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}


	/* 디버그 */
	/*FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = Hit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);*/

}

void ARCharacter::R_SkillHitCheck()
{
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	
	const float AttackRange = 400.0f;
	const float AttackDamage = 80.0f;
	
	TArray<FOverlapResult> OutOverlaps;
	UE_LOG(LogTemp, Display, TEXT("스킬 발동"));

	bool Hit = GetWorld()->OverlapMultiByChannel(OutOverlaps, GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRange), Params);
	if (Hit)
	{

		FDamageEvent DamageEvent;
		for (auto const& Overlap : OutOverlaps)
		{
			//DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 16, FColor::Green, false, 3.0f);
			Overlap.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
			UE_LOG(LogTemp, Display, TEXT("맞은 액터 이름 : %s"), *Overlap.GetActor()->GetActorNameOrLabel());
		}
	}
}

float ARCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void ARCharacter::OpenDoor()
{
	if (DoorUI->IsInViewport())
	{
		if (Door)
		{
			Door->SetCellDoor();
		}
	}
}

void ARCharacter::DisplayDoorUI(class AActor* InActor)
{
	if (InActor)
	{
		Door = Cast<ACellDoor>(InActor);
		Key = Cast<ARKey>(InActor);
	}

	if (DoorUI)
	{
		if (!DoorUI->IsInViewport())
		{
			DoorUI->AddToViewport();
		}
	}
}

void ARCharacter::UnDisplayDoorUI()
{
	if (DoorUI->IsInViewport())
	{
		DoorUI->RemoveFromViewport();
	}
}

void ARCharacter::SetHUD(URHUDWidget* HUDWidget)
{
	URHUDWidget* HUD = HUDWidget;
	if (HUD)
	{
		HUD->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnChange.AddUObject(HUD, &URHUDWidget::UpdateHpBar);
	}
}



