// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaStructTypes.h"
#include "GameFramework/SaveGame.h"
#include "DeltaSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSaveData
{
	GENERATED_BODY()

	FCharacterSaveData()
	{
		CharacterCurrentHealth = 0.0f;
		CharacterMaxHealth = 0.0f;
		CharacterMaxMana = 0.0f;
		CharacterCurrentMana = 0.0f;
	}
 
	UPROPERTY(SaveGame)
	FGuid CharacterGUID;
	
	UPROPERTY(SaveGame)
	FTransform CharacterTransform;
	
	UPROPERTY(SaveGame)
	float CharacterCurrentHealth;
	
	UPROPERTY(SaveGame)
	float CharacterMaxHealth;
	
	UPROPERTY(SaveGame)
	float CharacterCurrentMana;
	
	UPROPERTY(SaveGame)
	float CharacterMaxMana;
};

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
