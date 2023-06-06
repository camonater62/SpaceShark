// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroid.h"

// Sets default values
AAsteroid::AAsteroid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AsteroidVisualAsset(TEXT("/Game/Models/Planets/SM_Asteroid_New"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AsteroidVisualAsset2(TEXT("/Game/Models/Planets/SM_Asteroid2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SolarPanelVisualAsset(TEXT("/Game/Models/Debris/SM_SolarPanel"));

	float chance = FMath::FRand();

	if (AsteroidVisualAsset.Succeeded() && SolarPanelVisualAsset.Succeeded() && AsteroidVisualAsset2.Succeeded())
	{
		if (chance < 0.425)
		{
			VisualMesh->SetStaticMesh(AsteroidVisualAsset.Object);
		}
		else if (chance < 0.85)
		{
			VisualMesh->SetStaticMesh(AsteroidVisualAsset2.Object);
		}
		else
		{
			VisualMesh->SetStaticMesh(SolarPanelVisualAsset.Object);
		}
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		VisualMesh->Mobility = EComponentMobility::Movable;
		VisualMesh->SetSimulatePhysics(true);
	}
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
