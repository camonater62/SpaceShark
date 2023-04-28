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
		VisualMesh->OnComponentHit.AddDynamic(this, &AShark::OnHit);
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
			//UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *actor->GetName());
			if (actor && actor->GetName().StartsWith("BP_FirstPersonCharacter"))
			{
				//UE_LOG(LogTemp, Warning, TEXT("We found the character!"));
				BPFirstPerson = actor;
			}
		}
	}

}

// Called every frame
void AShark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if the Actor was found
	if (BPFirstPerson != nullptr && VisualMesh != nullptr)
	{
		// Pos
		FVector PlayerLocation = BPFirstPerson->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		FVector ToPlayer = (PlayerLocation - CurrentLocation).GetSafeNormal();

		// Force calcs
		const float ForceMag = 1000000;
		FVector Force = ForceMag * ToPlayer;
		FVector ForceOffset(0, 0, 0);

		const FName HeadBoneName = "jaw_master";

		int32 HeadBoneIndex = VisualMesh->GetBoneIndex(HeadBoneName);
		if (HeadBoneIndex != INDEX_NONE) {
			ForceOffset = VisualMesh->GetBoneLocation(HeadBoneName);
		}

		VisualMesh->AddForceAtLocation(ToPlayer * ForceMag, ForceOffset);

		// Log
		// UE_LOG(LogTemp, Warning, TEXT("BP_FirstPerson location: %s, Shark location: %s"), *PlayerLocation.ToString(), *CurrentLocation.ToString());
	}
}

void AShark::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	FName HitActorName = OtherActor->GetFName();
	UE_LOG(LogTemp, Warning, TEXT("We hit '%s'"), *HitActorName.ToString());
}