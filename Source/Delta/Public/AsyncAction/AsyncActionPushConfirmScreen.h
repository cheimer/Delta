// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionPushConfirmScreen.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClicked, EDeltaConfirmButtonType, ClickButtonType);

UCLASS()
class DELTA_API UAsyncActionPushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	virtual void Activate() override;
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirm Screen"))
	static UAsyncActionPushConfirmScreen* PushPushConfirmScreen(const UObject* WorldContextObject,
		EDeltaConfirmType ScreenType, FText InScreenTitle, FText InScreenMessage);

	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClicked OnConfirmScreenButtonClicked;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EDeltaConfirmType CachedScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMsg;
	
};
