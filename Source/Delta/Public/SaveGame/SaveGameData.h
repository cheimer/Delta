// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API USaveGameData : public USaveGame
{
	GENERATED_BODY()

public:
	FVector PlayerLocation;
	
};
