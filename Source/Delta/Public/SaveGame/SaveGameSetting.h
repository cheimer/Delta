// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameSetting.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API USaveGameSetting : public USaveGame
{
	GENERATED_BODY()

public:
	float VolumePercent;

	float MouseSensitivity;

	bool bFullscreen;
	
};
