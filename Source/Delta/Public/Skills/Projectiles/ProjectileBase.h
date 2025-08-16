// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CombatComponent.h"
#include "ProjectileBase.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DELTA_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	void SetDirection(const FVector& Direction) const;
	void SetGravityScale(const float GravityScale);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* ProjectileHitVFX;

	UPROPERTY(EditAnywhere, Category = "Values")
	float ProjectileDuration = 5.0f;
	
	float ProjectileDamage;

	TSet<TWeakObjectPtr<AActor>> ExcludeActors;

#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USphereComponent* ProjectileCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	TWeakObjectPtr<UCombatComponent> CombatComponent;

#pragma endregion Components

public:
	void SetProjectileDamage(const float Damage) {ProjectileDamage = Damage;}
	void SetCombatComponent(UCombatComponent* InCombatComponent) {CombatComponent = InCombatComponent;}
};
