// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGameInterface.generated.h"

class UDeltaSaveGame;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DELTA_API ISaveGameInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void SaveData(UDeltaSaveGame* DeltaSaveGame);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void LoadData(UDeltaSaveGame* DeltaSaveGame);
	
};
