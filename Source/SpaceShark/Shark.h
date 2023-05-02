// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shark.generated.h"

UCLASS()
class SPACESHARK_API AShark : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* VisualMesh;

public:
	// Sets default values for this actor's properties
	AShark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	AActor* BPFirstPerson;

};