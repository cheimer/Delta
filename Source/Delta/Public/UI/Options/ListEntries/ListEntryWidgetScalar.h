// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Options/ListEntries/ListEntryWidgetBase.h"
#include "ListEntryWidgetScalar.generated.h"

class UListDataObjectScalar;
class UAnalogSlider;
class UCommonNumericTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UListEntryWidgetScalar : public UListEntryWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason) override;

private:
	UFUNCTION()
	void OnSliderValueChanged(float Value);
	UFUNCTION()
	void OnSliderMouseCaptureBegin();
	
	UPROPERTY(Transient)
	UListDataObjectScalar* CachedOwningScalarDataObject;
	
#pragma region Bindings
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* SettingValueCommonNumericText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnalogSlider* SettingAnalogSlider;

#pragma endregion Bindings

};
