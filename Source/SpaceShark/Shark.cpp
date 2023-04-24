// Fill out your copyright notice in the Description page of Project Settings.


#include "Shark.h"
#include "Kismet/GameplayStatistics.h"

// Sets default values
AShark::AShark()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	FVector CurrentLocation = GetActorLocation();
	FVector PlayerLocation = player->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Player location is %s"), *PlayerLocation.ToString());
	CurrentLocation.X = FMath::Lerp(CurrentLocation.X, PlayerLocation.X, 0.01);
	SetActorLocation(CurrentLocation);
}

// Called to bind functionality to input
//void AShark::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

