// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "FrontGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UFrontGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UFrontGameUserSettings();
	
	static UFrontGameUserSettings* Get();

private:
	//~ Begin Gameplay Tab
	UPROPERTY(Config)
	EDeltaGameDifficulty CurrentGameDifficulty = EDeltaGameDifficulty::Normal;
	//~ End Gameplay Tab

	//~ Begin Audio Tab
	UPROPERTY(Config)
	float OverallVolume;
	UPROPERTY(Config)
	float BGMVolume;
	UPROPERTY(Config)
	float SFXVolume;
	
	UPROPERTY(Config)
	bool bAllowBackgroundAudio;
	UPROPERTY(Config)
	bool bUseHDRAudio;
	//~ End Audio Tab

public:
	UFUNCTION()
	EDeltaGameDifficulty GetCurrentGameDifficulty() const {return CurrentGameDifficulty;}
	UFUNCTION()
	void SetCurrentGameDifficulty(const EDeltaGameDifficulty InNewDifficulty) {CurrentGameDifficulty = InNewDifficulty;}

	UFUNCTION()
	FString GetCurrentGameDifficultyAsString() const;
	UFUNCTION()
	void SetCurrentGameDifficultyFromString(const FString& InDifficulty);

	UFUNCTION()
	float GetOverallVolume() const {return OverallVolume;}
	UFUNCTION()
	void SetOverallVolume(const float InOverallVolume);
	
	UFUNCTION()
	float GetBGMVolume() const {return BGMVolume;}
	UFUNCTION()
	void SetBGMVolume(const float InBGMVolume);
	
	UFUNCTION()
	float GetSFXVolume() const {return SFXVolume;}
	UFUNCTION()
	void SetSFXVolume(const float InSFXVolume);
	
	UFUNCTION()
	bool GetAllowBackgroundAudio() const {return bAllowBackgroundAudio;}
	UFUNCTION()
	void SetAllowBackgroundAudio(const bool InbAllowBackgroundAudio);
	
	UFUNCTION()
	bool GetUseHDRAudio() const {return bUseHDRAudio;}
	UFUNCTION()
	void SetUseHDRAudio(const bool InbUseHDRAudio);
	
	UFUNCTION()
	float GetCurrentDisplayGamma();
	UFUNCTION()
	void SetCurrentDisplayGamma(float InNewGamma);
	
};
