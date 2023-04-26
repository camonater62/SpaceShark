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

	// Get a reference to the current level
	ULevel* currentLevel = GetWorld()->GetCurrentLevel();

	// Iterate through all the Actors in the level and print their names
	// Find the Actor with the name "BP_FirstPerson"
	for (AActor* actor : currentLevel->Actors)
	{
		if (actor) {
			UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *actor->GetName());
			if (actor && actor->GetName().StartsWith("BP_FirstPersonCharacter"))
			{
				UE_LOG(LogTemp, Warning, TEXT("We found the character!"));
				bpFirstPerson = actor;
			}
		}
	}
	
}

// Called every frame
void AShark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if the Actor was found
	if (bpFirstPerson != nullptr)
	{
		// Get the location of the Actor
		FVector playerLocation = bpFirstPerson->GetActorLocation();

		// Assuming this code is inside an Actor class, get a reference to the current actor
		AActor* currentActor = Cast<AActor>(this);

		// Set the speed of movement towards the target location
		float speed = 0.1f;

		// Calculate the new position towards the target location using lerp
		FVector currentLocation = currentActor->GetActorLocation();
		const float forceMag = 1000000;
		FVector toPlayer = (playerLocation - currentLocation).GetSafeNormal();

		// Log
		UE_LOG(LogTemp, Warning, TEXT("BP_FirstPerson location: (%f, %f, %f)"), playerLocation.X, playerLocation.Y, playerLocation.Z);
		UE_LOG(LogTemp, Warning, TEXT("Shark location: (%f, %f, %f)"), currentLocation.X, currentLocation.Y, currentLocation.Z);

		// Movement
		if (!toPlayer.IsNearlyZero())
		{
			VisualMesh->AddForce(toPlayer * forceMag);
		}

		// Rotation
		/*FVector velocity = GetVelocity();
		if (!velocity.IsNearlyZero()) {
			FRotator newRotation = velocity.Rotation();
			currentActor->SetActorRotation(newRotation);
		}*/
	}

}

