// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Engine/DataAsset.h"
#include "InputDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FInputActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	ETriggerEvent TriggerEvent = ETriggerEvent::Triggered;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FName FunctionName;
	
};

UCLASS()
class DELTA_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TArray<FInputActionConfig> InputActionConfigs;
	
};
