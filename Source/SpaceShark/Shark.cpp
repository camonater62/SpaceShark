// Fill out your copyright notice in the Description page of Project Settings.

#include "Shark.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"

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
	ULevel *currentLevel = GetWorld()->GetCurrentLevel();

	// Iterate through all the Actors in the level and print their names
	// Find the Actor with the name "BP_FirstPerson"
	for (AActor *actor : currentLevel->Actors)
	{
		if (actor)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *actor->GetName());
			if (actor && actor->GetName().StartsWith("BP_FirstPersonCharacter"))
			{
				// UE_LOG(LogTemp, Warning, TEXT("We found the character!"));
				BPFirstPerson = actor;
			}
		}
	}

	// TArray<FName> BoneNames;
	// VisualMesh->GetBoneNames(BoneNames);
	// for (FName BoneName : BoneNames)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Bone Name: %s"), *BoneName.ToString());
	// 	int32 BoneIndex = VisualMesh->GetBoneIndex(BoneName);
	// 	FTransform BoneTransform = VisualMesh->GetBoneTransform(BoneIndex);

	// 	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(10.0f, 96.0f);
	// 	FTransform CapsuleTransform = BoneTransform;
	// 	CapsuleTransform.SetLocation(FVector::ZeroVector);
	// 	CapsuleTransform.SetScale3D(FVector::OneVector);
	// 	VisualMesh
	// }

	// Add a hit event
	VisualMesh->OnComponentHit.AddDynamic(this, &AShark::OnHit);

	AttackCooldown = 3;
	AttackTimer = AttackCooldown;

	DamageCooldown = 1;
	DamageTimer = DamageCooldown;
}

// Called every frame
void AShark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AttackTimer -= DeltaTime;
	DamageTimer -= DeltaTime;

	// Check if the Actor was found
	if (BPFirstPerson != nullptr && VisualMesh != nullptr)
	{
		// Pos
		FVector PlayerLocation = BPFirstPerson->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		FVector ToPlayer = PlayerLocation - CurrentLocation;
		FVector ToPlayerNormal = ToPlayer.GetSafeNormal();

		double Distance = FVector::Dist(PlayerLocation, CurrentLocation);
		if (Distance < 500 && DamageTimer < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("We hit the player!"));
			float dealt = BPFirstPerson->TakeDamage(10, FDamageEvent(), nullptr, this);
			UE_LOG(LogTemp, Warning, TEXT("We dealt %f damage"), dealt);
			DamageTimer = DamageCooldown;
		}

		if (Distance > 100000)
		{
			FVector NewLocation = PlayerLocation - 1000 * ToPlayerNormal;
			SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

			UE_LOG(LogTemp, Warning, TEXT("Teleporting shark! %d"), Distance);
		}
		else if (AttackTimer < 0)
		{
			SetActorLocation(CurrentLocation, false, nullptr, ETeleportType::ResetPhysics);

			// Force calcs
			const float ForceMag = 100000;
			FVector Force = ForceMag * ToPlayer;
			FVector ForceOffset(0, 0, 0);

			const FName HeadBoneName = "jaw_master";

			int32 HeadBoneIndex = VisualMesh->GetBoneIndex(HeadBoneName);
			if (HeadBoneIndex != INDEX_NONE)
			{
				ForceOffset = VisualMesh->GetBoneLocation(HeadBoneName);
			}

			VisualMesh->AddForceAtLocation(ToPlayer * ForceMag, ForceOffset);

			if (AttackTimer < -0.25)
			{
				AttackTimer = AttackCooldown;
			}
		}
		else
		{
			// Force calcs
			const float ForceMag = 10000;
			FVector Force = ForceMag * ToPlayer;
			FVector ForceOffset(0, 0, 0);

			const FName HeadBoneName = "jaw_master";

			int32 HeadBoneIndex = VisualMesh->GetBoneIndex(HeadBoneName);
			if (HeadBoneIndex != INDEX_NONE)
			{
				ForceOffset = VisualMesh->GetBoneLocation(HeadBoneName);
			}

			VisualMesh->AddForceAtLocation(ToPlayer * ForceMag, ForceOffset);
		}

		// Log
		// UE_LOG(LogTemp, Warning, TEXT("BP_FirstPerson location: %s, Shark location: %s"), *PlayerLocation.ToString(), *CurrentLocation.ToString());
	}
}

float AShark::TakeDamage(float Damage, struct FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	if (Health <= 0)
		return 0;

	float DamageCaused = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageCaused = FMath::Min(Health, DamageCaused);
	Health -= DamageCaused;

	UE_LOG(LogTemp, Warning, TEXT("Health left: %f"), Health);

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shark is dead!"));
		Destroy();
	}

	return DamageCaused;
}

void AShark::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	FName HitActorName = OtherActor->GetFName();
	UE_LOG(LogTemp, Warning, TEXT("We hit '%s'"), *HitActorName.ToString());
	FPointDamageEvent DamageEvent(10.0f, Hit, NormalImpulse, nullptr);
	// if (HitActorName.ToString().StartsWith("BP_FirstPersonCharacter"))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("We hit the player!"));
	// 	float dealt = OtherActor->TakeDamage(10.0f, DamageEvent, nullptr, OtherActor);
	// 	UE_LOG(LogTemp, Warning, TEXT("We dealt %f damage"), dealt);
	// }
	// else
	if (HitActorName.ToString().StartsWith("BP_FirstPersonProjectile"))
	{
		UE_LOG(LogTemp, Warning, TEXT("We got hit by a projectile!"));
		TakeDamage(10.0f, DamageEvent, nullptr, OtherActor);
		OtherActor->Destroy();
	}
}