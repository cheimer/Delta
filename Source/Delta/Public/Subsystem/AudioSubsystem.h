// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioSubsystem.generated.h"

class UAudioDeveloperSettings;

UENUM(BlueprintType)
enum class ESoundCategory : uint8
{
	Master,
	SFX,
	BGM
};

/**
 * 
 */
UCLASS()
class DELTA_API UAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UAudioSubsystem* Get(const UObject* WorldContextObject);

	void SetVolume(ESoundCategory SoundCategory, const float Volume);
	void SetUnfocusedVolume(bool bAllowUnfocusedVolume);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UPROPERTY()
	USoundMix* MasterSoundMix;

	UPROPERTY()
	TMap<ESoundCategory, USoundClass*> SoundClasses;

	UPROPERTY()
	const UAudioDeveloperSettings* CachedAudioSettings;
	
};
