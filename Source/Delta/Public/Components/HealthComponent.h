// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ADeltaBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth, bool, bIsDamaged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	void TakeDamage(float Damage);

	float GetHealthPercentage() const;
	void SetHealthPercentage(const float HealthPercentage);

	FOnHealthChanged OnHealthChanged;
	
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
	float GetMaxHealth() const {return MaxHealth;}
	float GetCurrentHealth() const {return CurrentHealth;}
	
#pragma endregion GetSet
	
};
