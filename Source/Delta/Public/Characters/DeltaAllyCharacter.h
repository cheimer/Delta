// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DeltaBaseCharacter.h"
#include "DeltaAllyCharacter.generated.h"

class ADeltaAllyController;

/**
 * DeltaAllyCharacter - Character class for AI-controlled or player-controlled allies
 * This class represents team members that can:
 * - Fight alongside the player
 * - Be controlled by AI when not possessed by player
 * - Be switched to for direct player control
 */
UCLASS()
class DELTA_API ADeltaAllyCharacter : public ADeltaBaseCharacter
{
	GENERATED_BODY()

public:
	ADeltaAllyCharacter();

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	// Update skill target for ally AI behavior
	virtual void UpdateSkillTarget() override;

	// Called when player switches control to this ally
	void OnPlayerControlStart();

	// Called when player switches control away from this ally
	void OnPlayerControlEnd();

	// Check if this ally is currently controlled by player
	bool IsPlayerControlled() const { return bIsPlayerControlled; }

protected:
	// Reference to AI controller for this ally
	UPROPERTY()
	ADeltaAllyController* AllyAIController;

	// Whether this ally is currently controlled by the player
	bool bIsPlayerControlled = false;

	// Previous controller before player took control (for restoration)
	UPROPERTY()
	AController* PreviousController;

public:
#pragma region GetSet

	ADeltaAllyController* GetAllyAIController() const { return AllyAIController; }

#pragma endregion GetSet
};
