// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DeltaGameInstance.generated.h"

class USaveGameSetting;
class USaveGameData;
/**
 * 
 */
UCLASS()
class DELTA_API UDeltaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UDeltaGameInstance();

	void SaveGameData();
	void LoadGameData();

	void SaveGameSetting();
	void LoadGameSetting();

private:
	void CreateSaveGameData();
	
	void CreateSaveGameSetting();
	
	UPROPERTY()
	USaveGameData* CurrentSaveData;
	UPROPERTY()
	USaveGameSetting* CurrentSaveSetting;

	FString SaveDataSlotName;
	FString SaveSettingSlotName;

	int32 UserIndex = 0;
};
