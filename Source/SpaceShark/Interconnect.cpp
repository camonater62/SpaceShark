// Fill out your copyright notice in the Description page of Project Settings.

#include "Interconnect.h"

// Sets default values
AInterconnect::AInterconnect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SharksDefeated = 0;
}

// Called when the game starts or when spawned
void AInterconnect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInterconnect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
