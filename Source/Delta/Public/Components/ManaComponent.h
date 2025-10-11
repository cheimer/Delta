// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"


class ADeltaBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, CurrentMana, float, MaxMana);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManaComponent();

	bool CanUseSkill(const float Amount) const;
	void UseSkill(const float Amount);

	void SetCurrentMana(const float NewMana);
	
	FOnManaChanged OnManaChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	void ManaRecoveryOnce();
	
private:
	UPROPERTY(EditAnywhere, Category = "Values")
	float MaxMana = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float ManaPerSecond = 0.5f;

	float CurrentMana;
	FTimerHandle ManaRecoveryTimer;
	
	UPROPERTY()
	ADeltaBaseCharacter* OwnerDeltaCharacter;

	float TickRate = 0.1f;

public:
#pragma region GetSet
	float GetCurrentMana() const {return CurrentMana;}
	float GetMaxMana() const {return MaxMana;}
	float GetManaPercentage ()const {return CurrentMana / MaxMana;}
	
#pragma endregion GetSet
	
};
