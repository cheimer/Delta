// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/DataObject/ListDataObjectStringResolution.h"

#include "GameUserSettings/FrontGameUserSettings.h"
#include "Helper/DeltaDebugHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Options/OptionsDataInteractionHelper.h"

void UListDataObjectStringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)->bool
		{
			return A.SizeSquared() < B.SizeSquared();
		}
	);
	
	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResToValueString(AvailableResolutions.Last());

	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObjectStringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResToDisplayText(UFrontGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObjectStringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"), InResolution.X, InResolution.Y);
}

FText UListDataObjectStringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	return FText::FromString(FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y));
}
