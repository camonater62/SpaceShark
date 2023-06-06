// Fill out your copyright notice in the Description page of Project Settings.

#include "SolarPanel.h"

// Sets default values
ASolarPanel::ASolarPanel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SolarPanelVisualAsset(TEXT("/Game/Models/Debris/SM_SolarPanel"));

	if (SolarPanelVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(SolarPanelVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		VisualMesh->Mobility = EComponentMobility::Movable;
		// VisualMesh->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		VisualMesh->SetSimulatePhysics(true);
	}
}

// Called when the game starts or when spawned
void ASolarPanel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASolarPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
