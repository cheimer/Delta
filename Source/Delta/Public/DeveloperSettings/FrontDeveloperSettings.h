// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FrontDeveloperSettings.generated.h"

class UActivatableBaseWidget;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Front UI Settings"))
class DELTA_API UFrontDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "WidgetReference", meta = (ForceInlineRow, Categories = "Front.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UActivatableBaseWidget>> FrontWidgetMap;

	UPROPERTY(Config, EditAnywhere, Category = "Options Image Reference", meta = (ForceInlineRow, Categories = "Front.Image"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImageMap;
	
};
