// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"


class ADeltaBaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManaComponent();

	void UseSkill(const float Amount);
	
	void StartManaRecoveryTimer();
	void StopManaRecoveryTimer();

protected:
	virtual void BeginPlay() override;

	void ManaRecoveryOnce();
	
private:
	UPROPERTY(EditAnywhere, Category = "Mana")
	float MaxMana = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Mana")
	float ManaRecoveryTime = 0.5f;
	
	float CurrentMana;
	FTimerHandle ManaRecoveryTimer;
	
	UPROPERTY()
	ADeltaBaseCharacter* OwnerDeltaCharacter;

public:
#pragma region GetSet
	float GetCurrentMana() const {return CurrentMana;}
	
#pragma endregion GetSet
	
};
