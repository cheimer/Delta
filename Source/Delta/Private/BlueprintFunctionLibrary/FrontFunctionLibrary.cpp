// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/FrontFunctionLibrary.h"

#include "DeveloperSettings/FrontDeveloperSettings.h"
#include "Helper/DeltaDebugHelper.h"

TSoftClassPtr<UActivatableBaseWidget> UFrontFunctionLibrary::GetFrontSoftWidgetClassByTag(UPARAM(meta = (Categories = "Front.Widget")) FGameplayTag InWidgetTag)
{
	const UFrontDeveloperSettings* FrontDeveloperSettings = GetDefault<UFrontDeveloperSettings>();
	
	if (FrontDeveloperSettings->FrontWidgetMap.Contains(InWidgetTag))
	{
		return FrontDeveloperSettings->FrontWidgetMap.FindRef(InWidgetTag);
	}
	else
	{
		DeltaDebug::Print("Cannot find widget under the tag : " + InWidgetTag.ToString());
		return nullptr;
	}
	
}

TSoftObjectPtr<UTexture2D> UFrontFunctionLibrary::GetOptionsSoftImageByTag(UPARAM(meta = (Categories = "Front.Image")) FGameplayTag InImageTag)
{
	const UFrontDeveloperSettings* FrontDeveloperSettings = GetDefault<UFrontDeveloperSettings>();

	if(FrontDeveloperSettings->OptionsScreenSoftImageMap.Contains(InImageTag))
	{
		return FrontDeveloperSettings->OptionsScreenSoftImageMap.FindRef(InImageTag);
	}
	else
	{
		DeltaDebug::Print("Cannot find image under the tag : " + InImageTag.ToString());
		return nullptr;
	}
}
