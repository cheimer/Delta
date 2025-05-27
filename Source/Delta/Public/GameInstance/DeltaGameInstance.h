// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DeltaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UDeltaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UDeltaGameInstance();

	virtual void Init() override;
	virtual void OnStart() override;
	virtual void Shutdown() override;

	void ApplySettings();

	void LoadFromFile();
	void SaveToFile();

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float MasterVolume;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool bIsFullscreen;
	
};
