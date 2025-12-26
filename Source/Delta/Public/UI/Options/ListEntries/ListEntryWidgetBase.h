// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "ListEntryWidgetBase.generated.h"

class UCommonTextBlock;
class UListDataObjectBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UListEntryWidgetBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (displayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);
	
protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Toggle Entry Widget Highlight State"))
	void BP_OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const;
	
	//~ Begin IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnEntryReleased() override;
	//~ End IUserObjectListEntry

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject);

	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason);
	virtual void OnOwningDependencyDataObjectModified(UListDataObjectBase* OwningModifiedDependencyData, EDeltaOptionsListDataModifyReasonType ModifyReason);

	virtual void OnToggleEditableState(bool bIsEditable);
	
	void SelectThisEntryWidget();

private:
	UPROPERTY(Transient)
	UListDataObjectBase* CachedOwningDataObject;
	
#pragma region Bindings
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* SettingDisplayNameCommonText;
		
#pragma endregion Bindings
	
};
