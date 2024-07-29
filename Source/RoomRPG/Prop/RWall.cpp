// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/RWall.h"
#include "Components/BoxComponent.h"
#include "Interface/RWallInterface.h"

// Sets default values
ARWall::ARWall()
{
    PrimaryActorTick.bCanEverTick = true;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetCollisionProfileName(TEXT("RTrigger"));
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARWall::OnWallBeginOverlap);
    RootComponent = Trigger;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Architecture/Crypt/SM_Crypt_Wall_Decorative_D.SM_Crypt_Wall_Decorative_D'"));
    if (MeshRef.Object)
    {
        Mesh->SetStaticMesh(MeshRef.Object);
    }
}

void ARWall::BeginPlay()
{
    Super::BeginPlay();

    bCheckkey = false;
    Start = GetActorLocation();
    MoveTime = 4.0f;
}

void ARWall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bCheckkey)
    {
        FVector Current = GetActorLocation();
        FVector Target = Start + FVector(0.0f, 0.0f, 390.0f);
        float Speed = FVector::Distance(Start, Target) / MoveTime;

        FVector NewLocation = FMath::VInterpConstantTo(Current, Target, DeltaTime, Speed);
        SetActorLocation(NewLocation);
    }
}

void ARWall::OnWallBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    IRWallInterface* Player = Cast<IRWallInterface>(OtherActor);
    if (Player)
    {
        bCheckkey = Player->CheckKey();
        if (bCheckkey)
        {
            Player->WatchWall();
        }
    }
}




