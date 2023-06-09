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
	BloodComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Blood"));
	ExplosionComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Explosion"));
	Damaged1 = CreateDefaultSubobject<UMaterial>(TEXT("Damaged1"));
	Damaged2 = CreateDefaultSubobject<UMaterial>(TEXT("Damaged2"));
	Damaged3 = CreateDefaultSubobject<UMaterial>(TEXT("Damaged3"));
	VisualMesh->SetupAttachment(RootComponent);
	BloodComp->SetupAttachment(RootComponent);
	ExplosionComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SharkVisualAsset(TEXT("/Game/Models/Shark/shark"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BloodEffect(TEXT("/Game/Effects/Blood/P_BloodSplatter"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ExplosionEffect(TEXT("/Game/Effects/Explosion/M_Explosion_Particles"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material1(TEXT("/Game/Models/Shark/shark_damaged_Mat"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material2(TEXT("/Game/Models/Shark/shark_damaged2_Mat"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material3(TEXT("/Game/Models/Shark/shark_damaged3_Mat"));

	if (SharkVisualAsset.Succeeded())
	{
		VisualMesh->SetSkeletalMesh(SharkVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	if (BloodEffect.Succeeded() && ExplosionEffect.Succeeded())
	{
		BloodComp->SetAsset(BloodEffect.Object);
		ExplosionComp->SetAsset(ExplosionEffect.Object);
	}
	if (Material1.Succeeded() && Material2.Succeeded() && Material3.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Materials Succeded"));
		Damaged1 = Material1.Object;
		Damaged2 = Material2.Object;
		Damaged3 = Material3.Object;
	}

	Stage = 0;
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

	// Enable physics on everything
	SetActorScale3D(FVector(100));
	VisualMesh->SetAllBodiesSimulatePhysics(true);
	VisualMesh->SetSimulatePhysics(true);
	VisualMesh->SetEnableGravity(true);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Add a hit event
	VisualMesh->OnComponentHit.AddDynamic(this, &AShark::OnHit);

	AttackTimer = 2 * ATTACK_COOLDOWN;
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
		FVector CurrentVelocity = VisualMesh->GetPhysicsLinearVelocity();
		FVector CurrentAngularVelocity = VisualMesh->GetPhysicsAngularVelocityInDegrees();

		double Distance = FVector::Dist(PlayerLocation, CurrentLocation);
		if (AttackTimer > ATTACK_COOLDOWN)
		{
			// Do nothing
			// This gives the player some time to react when the game starts
		}
		if (Distance < 500 && DamageTimer < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("We hit the player!"));
			float dealt = BPFirstPerson->TakeDamage(10, FDamageEvent(), nullptr, this);
			UE_LOG(LogTemp, Warning, TEXT("We dealt %f damage"), dealt);
			DamageTimer = DAMAGE_COOLDOWN;
		}

		if (Distance > MAX_DISTANCE)
		{
			FVector NewLocation = PlayerLocation - 0.85f * MAX_DISTANCE * ToPlayerNormal;
			SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

			UE_LOG(LogTemp, Warning, TEXT("Teleporting shark! %d"), Distance);
		}
		else if (AttackTimer < 0)
		{
			VisualMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

			// Force calcs
			const float ForceMag = 25000 * MOVEMENT_SPEED;
			FVector Force = ForceMag * ToPlayerNormal;
			FVector ForceOffset(0, 0, 0);

			const FName HeadBoneName = "jaw_master";

			int32 HeadBoneIndex = VisualMesh->GetBoneIndex(HeadBoneName);
			if (HeadBoneIndex != INDEX_NONE)
			{
				ForceOffset = VisualMesh->GetBoneLocation(HeadBoneName);
			}

			VisualMesh->AddForceAtLocation(Force, ForceOffset);

			if (AttackTimer < -0.25)
			{
				AttackTimer = ATTACK_COOLDOWN;
			}
		}
		else if (CurrentVelocity.Length() > MOVEMENT_SPEED || 10 * CurrentAngularVelocity.Length() > MOVEMENT_SPEED)
		{
			UE_LOG(LogTemp, Warning, TEXT("Shark is moving too fast!"));
			VisualMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
			VisualMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		}
		else
		{
			// Default state

			// Force calcs
			FVector Force = MOVEMENT_SPEED * 500 * ToPlayerNormal;
			FVector ForceOffset(0, 0, 0);

			const FName HeadBoneName = "jaw_master";

			int32 HeadBoneIndex = VisualMesh->GetBoneIndex(HeadBoneName);
			if (HeadBoneIndex != INDEX_NONE)
			{
				ForceOffset = VisualMesh->GetBoneLocation(HeadBoneName);
			}

			VisualMesh->AddForceAtLocation(Force, ForceOffset);
		}
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

	BloodComp->SetRelativeLocationAndRotation(RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
	BloodComp->ActivateSystem();
	if (Health <= 0)
	{
		BPFirstPerson->TakeDamage(-20, FDamageEvent(), nullptr, this);
		if (Stage >= 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("Shark is dead!"));
			ExplosionComp->SetRelativeLocationAndRotation(RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
			ExplosionComp->ActivateSystem();
			Destroy();
			if (GetNumberOfSharksInLevel() == 0)
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName("WinScreen"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Shark is dead!"));
			Stage++;

			// Spawn a new shark
			FVector NewLocation = GetActorLocation() + FVector(0, 0, 100);
			AShark *NewShark = GetWorld()->SpawnActor<AShark>(AShark::StaticClass(), NewLocation, GetActorRotation());
			NewShark->SetActorScale3D(GetActorScale3D());
			NewShark->Stage = Stage;
			MAX_HEALTH /= 2;
			NewShark->MAX_HEALTH = MAX_HEALTH;
			NewShark->Health = MAX_HEALTH;
			Health = MAX_HEALTH;
		}
	}
	if (Health <= (MAX_HEALTH * 0.25))
	{
		VisualMesh->SetMaterial(0, Damaged3);
	}
	else if (Health <= (MAX_HEALTH * 0.5))
	{
		VisualMesh->SetMaterial(0, Damaged2);
	}
	else if (Health <= (MAX_HEALTH * 0.75))
	{
		VisualMesh->SetMaterial(0, Damaged1);
	}
	return DamageCaused;
}

void AShark::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	FName HitActorName = OtherActor->GetFName();
	UE_LOG(LogTemp, Warning, TEXT("We hit '%s'"), *HitActorName.ToString());
	FPointDamageEvent DamageEvent(10.0f, Hit, NormalImpulse, nullptr);

	if (HitActorName.ToString().StartsWith("BP_FirstPersonProjectile"))
	{
		UE_LOG(LogTemp, Warning, TEXT("We got hit by a projectile!"));
		TakeDamage(10.0f, DamageEvent, nullptr, OtherActor);
		OtherActor->Destroy();
	}
}

int AShark::GetNumberOfSharksInLevel()
{
	ULevel *Level = GetWorld()->GetCurrentLevel();
	TArray<AActor *> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShark::StaticClass(), Actors);
	return Actors.Num();
}