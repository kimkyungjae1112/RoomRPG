// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/CellDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Interface/RInteractionDoorInterface.h"

// Sets default values
ACellDoor::ACellDoor()
{
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    CellFront = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell_Front"));
    CellDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell_Door"));

    //Trigger
    Trigger->SetupAttachment(RootComponent);
    Trigger->SetCollisionProfileName(TEXT("Door"));
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACellDoor::OnDoorBeginOverlap);
    Trigger->OnComponentEndOverlap.AddDynamic(this, &ACellDoor::OnDoorEndOverlap);

    //Static Mesh
    CellFront->SetupAttachment(Trigger);
    CellDoor->SetupAttachment(Trigger);
    CellDoor->SetRelativeRotation(FRotator::ZeroRotator);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CellFrontMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Architecture/Dungeon/SM_Cell_Front.SM_Cell_Front'"));
    if (CellFrontMeshRef.Object)
    {
        CellFront->SetStaticMesh(CellFrontMeshRef.Object);
    }
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CellDoorMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Architecture/Dungeon/SM_Cell_Door.SM_Cell_Door'"));
    if (CellDoorMeshRef.Object)
    {
        CellDoor->SetStaticMesh(CellDoorMeshRef.Object);
    }
}

void ACellDoor::OnDoorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    IRInteractionDoorInterface* Player = Cast<IRInteractionDoorInterface>(OtherActor);
    if (Player)
    {
        Player->DisplayDoorUI(this);
    }
}

void ACellDoor::OnDoorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    IRInteractionDoorInterface* Player = Cast<IRInteractionDoorInterface>(OtherActor);
    if (Player)
    {
        Player->UnDisplayDoorUI();
    }
}

void ACellDoor::SetCellDoor()
{
    UE_LOG(LogTemp, Display, TEXT("x : %f, y : %f, z : %f"), CellDoor->GetRelativeRotation().Pitch, CellDoor->GetRelativeRotation().Yaw, CellDoor->GetRelativeRotation().Roll);
    if (CellDoor->GetRelativeRotation().Yaw > -1.0f)
    {
        UE_LOG(LogTemp, Display, TEXT("열어"));
        CellDoor->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("닫어"));
        CellDoor->SetRelativeRotation(FRotator::ZeroRotator);
    }
}

