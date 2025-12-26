// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionPushSoftWidget.generated.h"

class UActivatableBaseWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidget, UActivatableBaseWidget*, PushedWidget);

/**
 * 
 */
UCLASS()
class DELTA_API UAsyncActionPushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To  Widget Stack"))
	static UAsyncActionPushSoftWidget* PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<UActivatableBaseWidget> InSoftWidgetClass,
		UPARAM(meta = (Categories = "Front.WidgetSTack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget = true);

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidget OnBeforePushSoftWidget;
	
	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidget OnAfterPushSoftWidget;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	TSoftClassPtr<UActivatableBaseWidget> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
	
};
