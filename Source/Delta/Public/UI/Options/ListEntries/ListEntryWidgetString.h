// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Options/ListEntries/ListEntryWidgetBase.h"
#include "ListEntryWidgetString.generated.h"

class UListDataObjectString;
class UFrontCommonRotator;
class UFrontCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UListEntryWidgetString : public UListEntryWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason) override;
	
	virtual void OnToggleEditableState(bool bIsEditable) override;

private:
	void OnBeforeOptionButtonClicked();
	void OnNextOptionButtonClicked();

	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);
	
	UPROPERTY(Transient)
	UListDataObjectString* CachedOwningStringDataObject;
	
#pragma region Bindings
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontCommonButtonBase* BeforeOptionCommonButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontCommonRotator* AvailableOptionsCommonRotator;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontCommonButtonBase* NextOptionCommonButton;

#pragma endregion Bindings

};
