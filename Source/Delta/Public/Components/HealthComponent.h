// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ADeltaBaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	void TakeDamage(float Damage);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;
	
	UPROPERTY()
	ADeltaBaseCharacter* OwnerDeltaCharacter;
	
	float CurrentHealth;
	bool bIsDead = false;
	
public:
#pragma region GetSet
	bool GetIsDead() const {return bIsDead;}
	
#pragma endregion GetSet
	
};
