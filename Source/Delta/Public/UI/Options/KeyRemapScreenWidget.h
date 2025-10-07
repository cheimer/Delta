// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ActivatableBaseWidget.h"
#include "CommonInputTypeEnum.h"
#include "KeyRemapScreenWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressed, const FKey& /*PressedKey*/)
DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceled, const FString& /*CanceledReason*/)

class UCommonRichTextBlock;
class FKeyRemapScreenInputProcessor;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UKeyRemapScreenWidget : public UActivatableBaseWidget
{
	GENERATED_BODY()

public:
	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);

	FOnKeyRemapScreenKeyPressed OnKeyRemapScreenKeyPressed;
	FOnKeyRemapScreenKeySelectCanceled OnKeyRemapScreenKeySelectCanceled;

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	void OnValidKeyPressedDetected(const FKey& PressedKey);
	void OnKeySelectCanceled(const FString& CanceledReason);

	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);
	
	TSharedPtr<FKeyRemapScreenInputProcessor> CachedInputPreProcessor;

	ECommonInputType CachedDesiredInputType;
	
#pragma region Bindings
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* RemapMessageCommonRichText;

#pragma endregion Bindings
	
};
