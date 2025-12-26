// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontUISubsystem.generated.h"

class UFrontCommonButtonBase;
class UActivatableBaseWidget;
struct FGameplayTag;
class UPrimaryLayOutWidget;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdated, UFrontCommonButtonBase*, FrontCommonButtonBase, FText, DescriptionText);

/**
 * 
 */
UCLASS()
class DELTA_API UFrontUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UFrontUISubsystem* Get(const UObject* WorldContextObject);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayout(UPrimaryLayOutWidget* InPrimaryLayOutWidget);

	void PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag, TSoftClassPtr<UActivatableBaseWidget> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState, UActivatableBaseWidget*)> AsyncPushStateCallback);
	void PushConfirmScreenToModalStackAsync(EDeltaConfirmType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg,
		TFunction<void(EDeltaConfirmButtonType)>ButtonClickedCallback);

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdated OnButtonDescriptionTextUpdated;

private:
	UPROPERTY(Transient)
	UPrimaryLayOutWidget* CreatedPrimaryLayout;
	
};
