// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/RBomb.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Interface/RBossBombInterface.h"

// Sets default values
ARBomb::ARBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetCollisionProfileName(TEXT("RTrigger"));
	Trigger->SetActive(false);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileComponent->UpdatedComponent = Mesh;
	ProjectileComponent->InitialSpeed = 1000.f;
	ProjectileComponent->MaxSpeed = 1000.f;
	ProjectileComponent->bRotationFollowsVelocity = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FirstFireRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/M5VFXVOL2/Niagara/Fire_for_BP/NFire_BP_00.NFire_BP_00'"));
	if (FirstFireRef.Object)
	{
		FirstFire = FirstFireRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ExplosionRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/M5VFXVOL2/Niagara/Explosion/NFire_Exp_00.NFire_Exp_00'"));
	if (ExplosionRef.Object)
	{
		Explosion = ExplosionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/M5VFXVOL2/Niagara/Fire/NFire_08.NFire_08'"));
	if (NiagaraRef.Object)
	{
		GroundFire = NiagaraRef.Object;
	}

	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetAsset(FirstFire);
	NiagaraComponent->bAutoActivate = true;
}

// Called when the game starts or when spawned
void ARBomb::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->OnComponentHit.AddDynamic(this, &ARBomb::OnHit);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARBomb::OnBombBeginOverlap);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			Destroy();
		}, 8.0f, false);
}

// Called every frame
void ARBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARBomb::SetDirection(const FVector& InDirection)
{
	ProjectileComponent->Velocity = InDirection * ProjectileComponent->InitialSpeed;
}

void ARBomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bFlag)
	{
		bFlag = false;
		Mesh->SetSimulatePhysics(false);
		Mesh->SetHiddenInGame(true);
		Trigger->SetActive(true);
		NiagaraComponent->SetAsset(Explosion);

		const float Damage = 75.0f;
		IRBossBombInterface* Boss = Cast<IRBossBombInterface>(GetOwner());
		if (Boss)
		{
			Boss->TakeDamageByBomb(OtherActor, Damage);
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				NiagaraComponent->SetAsset(GroundFire);
			}, 2.5f, false);		
	}
}

void ARBomb::OnBombBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float Distance = FVector::Distance(GetActorLocation(), OtherActor->GetActorLocation());
	float Damage = (1 / Distance) * 7000;
	UE_LOG(LogTemp, Display, TEXT("Overlap Damage : %f"), Damage);
		
	IRBossBombInterface* Boss = Cast<IRBossBombInterface>(GetOwner());
	if (Boss)
	{
		Boss->TakeDamageByBomb(OtherActor, Damage);
	}
}

