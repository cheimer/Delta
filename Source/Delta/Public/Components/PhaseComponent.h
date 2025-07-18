// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhaseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, int, PhaseNum);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UPhaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPhaseComponent();

	FOnPhaseChanged OnPhaseChanged;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(float CurrentHealth, float MaxHealth, bool bIsDamaged);

private:
	UPROPERTY(EditAnywhere, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TArray<float> PhaseTransRates;

	int CurrentPhase = 0;

public:
	int GetPhaseRateNum() const { return PhaseTransRates.Num(); }
	int GetCurrentPhaseNum() const { return CurrentPhase; }
		
};
