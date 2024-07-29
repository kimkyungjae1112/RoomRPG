// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/RKey.h"
#include "Components/CapsuleComponent.h"
#include "Interface/RInteractionDoorInterface.h"

// Sets default values
ARKey::ARKey()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleSize(200.0f, 200.0f);
	Capsule->SetCollisionProfileName(TEXT("RTrigger"));

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ARKey::OnKeyBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ARKey::OnKeyEndOverlap);
	RootComponent = Capsule;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Props/SM_Gargoyle_Statue.SM_Gargoyle_Statue'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
		Mesh->SetCollisionProfileName(TEXT("Item"));
	}
}

void ARKey::BeginPlay()
{
	Super::BeginPlay();

}

void ARKey::OnKeyBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IRInteractionDoorInterface* KeyInterface = Cast<IRInteractionDoorInterface>(OtherActor);
	if (KeyInterface)
	{
		KeyInterface->DisplayDoorUI(this);
	}
}

void ARKey::OnKeyEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IRInteractionDoorInterface* KeyInterface = Cast<IRInteractionDoorInterface>(OtherActor);
	if (KeyInterface)
	{
		KeyInterface->UnDisplayDoorUI();
	}
}
