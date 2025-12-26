// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FrontFunctionLibrary.generated.h"

class UActivatableBaseWidget;
/**
 * 
 */
UCLASS()
class DELTA_API UFrontFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Front Function Library")
	static TSoftClassPtr<UActivatableBaseWidget> GetFrontSoftWidgetClassByTag(
		UPARAM(meta = (Categories = "Front.Widget")) FGameplayTag InWidgetTag);
	
	UFUNCTION(BlueprintPure, Category = "Front Function Library")
	static TSoftObjectPtr<UTexture2D> GetOptionsSoftImageByTag(UPARAM(meta = (Categories = "Front.Image")) FGameplayTag InImageTag);
	
};
