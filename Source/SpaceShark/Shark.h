// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Interconnect.h"
#include "Shark.generated.h"

UCLASS()
class SPACESHARK_API AShark : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *VisualMesh;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent *BloodComp;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent *ExplosionComp;

public:
	// Sets default values for this actor's properties
	AShark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const float ATTACK_DAMAGE = 10;
	const float ATTACK_RANGE = 100;
	const float ATTACK_COOLDOWN = 3;
	const float DAMAGE_COOLDOWN = 1;
	float MAX_HEALTH = 80;
	const float MOVEMENT_SPEED = 10000;
	const float MAX_DISTANCE = 100000;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = MAX_HEALTH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Stage;

private:
	AActor *BPFirstPerson;
	AInterconnect *Interconnect;
	float DamageTimer;
	bool Endless;

	int GetNumberOfSharksInLevel();
	UPROPERTY(VisibleAnywhere)
	UTexture *Damaged1;
	UPROPERTY(VisibleAnywhere)
	UTexture *Damaged2;
	UPROPERTY(VisibleAnywhere)
	UTexture *Damaged3;
	UTexture *SharkTexture;
	UMaterialInterface *SharkMat;
	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic *SharkDynamicMaterial;
};