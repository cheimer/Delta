// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaStructTypes.h"
#include "GameFramework/SaveGame.h"
#include "DeltaSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UDeltaSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/**
	 * Time
	 */
	UPROPERTY(SaveGame)
	FDateTime SavedTime;
	UPROPERTY(SaveGame)
	float PlayingTime;

	/**
	 * Character
	 */
	UPROPERTY(SaveGame)
	FCharacterSaveData CharacterSaveData;
	
};
