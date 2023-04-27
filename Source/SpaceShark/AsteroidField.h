// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AsteroidField.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACESHARK_API UAsteroidField : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAsteroidField();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, Category="Asteroid Field", DisplayName="Number of Asteroids")
	int numAsteroids = 0;
	UPROPERTY(EditAnywhere, Category = "Asteroid Field", DisplayName = "Asteroid Field Size")
	FVector size = FVector(0.0f, 0.0f, 0.0f);
		
};
