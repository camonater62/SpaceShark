// Fill out your copyright notice in the Description page of Project Settings.


#include "Shark.h"

// Sets default values
AShark::AShark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisualMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SharkVisualAsset(TEXT("/Game/Models/Shark/shark"));

	if (SharkVisualAsset.Succeeded()) 
	{
		VisualMesh->SetSkeletalMesh(SharkVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AShark::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = GetActorLocation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.X += DeltaHeight * 20.0f;
	float DeltaRotation = DeltaTime * 20.0f;
	SetActorLocation(NewLocation);

}

