// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DeltaGameState.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API ADeltaGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	bool bIsGamePaused = false;

public:
#pragma region GetSet
	bool GetIsGamePaused() const {return bIsGamePaused;}
	void SetGamePause(const bool InGamePaused) {bIsGamePaused = InGamePaused;}

#pragma endregion GetSet
	
};
