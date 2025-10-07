// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Subsystem/AudioSubsystem.h"
#include "AudioDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Audio Settings"))
class DELTA_API UAudioDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Audio")
	TSoftObjectPtr<USoundMix> MasterSoundMix;

	UPROPERTY(Config, EditAnywhere, Category = "Audio")
	TMap<ESoundCategory, TSoftObjectPtr<USoundClass>> SoundClasses;

	UPROPERTY(Config, EditAnywhere, Category = "Test Sounds")
	TMap<ESoundCategory, TSoftObjectPtr<USoundBase>> TestSounds;
	
};
