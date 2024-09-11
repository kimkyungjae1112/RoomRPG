// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/RBossRoom.h"
#include "Components/BoxComponent.h"
#include "Stat/RBossStat.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARBossRoom::ARBossRoom()
{
	EnterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterTrigger"));
	EnterTrigger->SetCollisionProfileName(TEXT("RTirgger"));
	EnterTrigger->SetupAttachment(RootComponent);
	EnterTrigger->OnComponentEndOverlap.AddDynamic(this, &ARBossRoom::OnBossRoomEndOverlap);
	EnterTrigger->SetBoxExtent(FVector(10.0f, 115.0f, 100.0f));

	WinTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("WinTrigger"));
	WinTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	WinTrigger->SetupAttachment(RootComponent);
	WinTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARBossRoom::OnWinRoomOverlap);

	DoorWay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorWay"));
	DoorLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeft"));
	DoorRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRight"));

	DoorWay->SetupAttachment(EnterTrigger);
	DoorLeft->SetupAttachment(DoorWay);
	DoorLeft->SetRelativeRotation(FRotator(0.0f, 110.0f, 0.0f));
	DoorLeft->SetRelativeLocation(FVector(0.0f, -130.0f, 0.0f));
	DoorRight->SetupAttachment(DoorWay);
	DoorRight->SetRelativeRotation(FRotator(0.0f, -110.0f, 0.0f));
	DoorRight->SetRelativeLocation(FVector(0.0f, 130.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorWayRef(TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Architecture/Dungeon/SM_DoorWay.SM_DoorWay'"));
	if (DoorWayRef.Object)
	{
		DoorWay->SetStaticMesh(DoorWayRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorLeftRef(TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Architecture/Dungeon/SM_DoorWay_Large_Door_Left.SM_DoorWay_Large_Door_Left'"));
	if (DoorLeftRef.Object)
	{
		DoorLeft->SetStaticMesh(DoorLeftRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorRightRef(TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Architecture/Dungeon/SM_DoorWay_Large_Door_Right.SM_DoorWay_Large_Door_Right'"));
	if (DoorRightRef.Object)
	{
		DoorRight->SetStaticMesh(DoorRightRef.Object);
	}

}

// Called when the game starts or when spawned
void ARBossRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARBossRoom::OnBossRoomEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DoorLeft->SetRelativeRotation(FRotator::ZeroRotator);
	DoorRight->SetRelativeRotation(FRotator::ZeroRotator);

	OnEnterBossRoom.ExecuteIfBound();
}

void ARBossRoom::OnWinRoomOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::OpenLevel(this, FName("Demonstration"));
}

void ARBossRoom::SetWinTrigger()
{
	EnterTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	WinTrigger->SetCollisionProfileName(TEXT("RTrigger"));
	DoorLeft->SetRelativeRotation(FRotator(0.0f, 110.0f, 0.0f));
	DoorRight->SetRelativeRotation(FRotator(0.0f, -110.0f, 0.0f));
}




