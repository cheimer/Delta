// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ActivatableBaseWidget.generated.h"

class ADeltaFrontPlayerController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UActivatableBaseWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	ADeltaFrontPlayerController* GetFrontPlayerController();
	
private:
	TWeakObjectPtr<ADeltaFrontPlayerController> CachedOwningFrontPC;
	
};
