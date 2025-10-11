// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

class UDeltaSaveGame;
/**
 * 
 */
UCLASS()
class DELTA_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static USaveGameSubsystem* Get(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool SaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool LoadGame();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool HaveSaveFile() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	FText GetSaveDate() const;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UPROPERTY(Transient)
	UDeltaSaveGame* CurrentSaveGame;

	FString SaveSlotString;
	int32 SaveUserIndex;


};
