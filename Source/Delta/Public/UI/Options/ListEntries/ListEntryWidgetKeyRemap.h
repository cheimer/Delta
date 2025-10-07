// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Options/ListEntries/ListEntryWidgetBase.h"
#include "ListEntryWidgetKeyRemap.generated.h"

class UListDataObjectKeyRemap;
class UFrontCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UListEntryWidgetKeyRemap : public UListEntryWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject) override;
	
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason) override;

private:
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();

	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyToRemapCanceled(const FString& CanceledReason);
	
	UPROPERTY(Transient)
	UListDataObjectKeyRemap* CachedOwningKeyRemapDataObject;

#pragma region Bindings
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontCommonButtonBase* RemapKeyCommonButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontCommonButtonBase* ResetKeyCommonButton;
	
#pragma endregion Bindings
	
};
