// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ADeltaBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, ChangedActor, float, CurrentHealth, float, MaxHealth, bool, bIsDamaged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	void TakeDamage(float Damage, AActor* DamageCauser);

	float GetHealthPercentage() const;
	void SetHealthPercentage(float HealthPercentage);

	bool CauseDamage(const float Damage);

	FOnHealthChanged OnHealthChanged;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;
	
	UPROPERTY()
	ADeltaBaseCharacter* OwnerDeltaCharacter;

	UPROPERTY(Transient)
	float CurrentHealth;
	bool bIsDead = false;
	
public:
#pragma region GetSet
	bool GetIsDead() const {return bIsDead;}
	float GetMaxHealth() const {return MaxHealth;}
	void SetMaxHealth(const float InMaxHealth);
	float GetCurrentHealth() const {return CurrentHealth;}

#pragma endregion GetSet
	
};
