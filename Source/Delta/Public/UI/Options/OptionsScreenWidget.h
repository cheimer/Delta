// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "UI/ActivatableBaseWidget.h"
#include "OptionsScreenWidget.generated.h"

class UListDataObjectBase;
class UOptionsDetailWidget;
class UFrontCommonListView;
class UFrontTabListWidgetBase;
class UOptionsDataRegistry;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UOptionsScreenWidget : public UActivatableBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

	void OnListViewListDataModified(UListDataObjectBase* ModifiedData, EDeltaOptionsListDataModifyReasonType ModifiedReason);

	UPROPERTY(Transient)
	UOptionsDataRegistry* OwningDataRegistry;
	
	UPROPERTY(EditDefaultsOnly, Category = "Front Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	UPROPERTY(Transient)
	TArray<UListDataObjectBase*> ResettableDataArray;

	FUIActionBindingHandle ResetActionHandle;

	bool bIsResettingData = false;
	
#pragma region Bindings
	UPROPERTY(meta = (BindWidget))
	UFrontTabListWidgetBase* OptionsTabListWidget;
	
	UPROPERTY(meta = (BindWidget))
	UFrontCommonListView* OptionsListView;
	
	UPROPERTY(meta = (BindWidget))
	UOptionsDetailWidget* ListEntryInfoDetailsView;
	
#pragma endregion Bindings

};
