// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Options/DataObject/ListDataObjectString.h"
#include "ListDataObjectStringResolution.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UListDataObjectStringResolution : public UListDataObjectString
{
	GENERATED_BODY()

public:
	void InitResolutionValues();
	
protected:
	virtual void OnDataObjectInitialized() override;

private:
	FString ResToValueString(const FIntPoint& InResolution) const;
	FText ResToDisplayText(const FIntPoint& InResolution) const;

	FString MaximumAllowedResolution;

public:
	FString GetMaximumAllowedResolution() const {return MaximumAllowedResolution;}

};
