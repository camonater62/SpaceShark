// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interconnect.generated.h"

UCLASS()
class SPACESHARK_API AInterconnect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInterconnect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SharksDefeated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Endless;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
