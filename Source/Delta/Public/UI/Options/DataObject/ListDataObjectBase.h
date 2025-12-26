// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "DeltaTypes/DeltaStructTypes.h"
#include "UObject/NoExportTypes.h"
#include "ListDataObjectBase.generated.h"

class UListDataObjectBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModified, UListDataObjectBase*, EDeltaOptionsListDataModifyReasonType);

/**
 * 
 */
UCLASS(Abstract)
class DELTA_API UListDataObjectBase : public UObject
{
	GENERATED_BODY()

public:
	void InitDataObject();
	
	virtual TArray<UListDataObjectBase*> GetAllChildListData() const {return TArray<UListDataObjectBase*>();}
	virtual bool HasAnyChildListData() const {return false;}

	void SetShouldApplySettingsImmediately(bool bShouldApply) {bShouldApplyChangeImmediately = bShouldApply;}

	virtual bool HasDefaultValue() const {return false;}
	virtual bool CanResetBackToDefaultValue() const {return false;}
	virtual bool TryResetBackToDefaultValue() {return false;}

	void AddEditCondition(const FOptionsDataEditCondition& InEditCondition);
	void AddEditDependencyData(UListDataObjectBase* InEditDependencyData);
	
	bool IsDataCurrentlyEditable();

	FOnListDataModified OnListDataModified;
	FOnListDataModified OnDependencyDataModified;

protected:
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(UListDataObjectBase* ModifiedData,
		EDeltaOptionsListDataModifyReasonType ModifyReason = EDeltaOptionsListDataModifyReasonType::DirectlyModified);

	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const {return false;}
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) {}

	virtual void OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EDeltaOptionsListDataModifyReasonType ModifyReason);
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UListDataObjectBase* ParentData;

	bool bShouldApplyChangeImmediately = false;

	UPROPERTY(Transient)
	TArray<FOptionsDataEditCondition> EditConditions;

public:
#pragma region GetSet
	FName GetDataID() const{return DataID;}
	void SetDataID(const FName& InDataID){DataID = InDataID;}

	FText GetDataDisplayName() const{return DataDisplayName;}
	void SetDataDisplayName(const FText& InDataDisplayName){DataDisplayName = InDataDisplayName;}

	FText GetDescriptionRichText() const{return DescriptionRichText;}
	void SetDescriptionRichText(const FText& InDescriptionRichText){DescriptionRichText = InDescriptionRichText;}

	FText GetDisabledRichText() const{return DisabledRichText;}
	void SetDisabledRichText(const FText& InDisabledRichText){DisabledRichText = InDisabledRichText;}

	TSoftObjectPtr<UTexture2D> GetSoftDescriptionImage() const {return SoftDescriptionImage;}
	void SetSoftDescriptionImage (const TSoftObjectPtr<UTexture2D>& InSoftDescriptionImage){SoftDescriptionImage = InSoftDescriptionImage;}

	UListDataObjectBase* GetParentData() const {return ParentData;}
	void SetParentData(UListDataObjectBase* const InParentData) {ParentData = InParentData;}

#pragma endregion GetSet
	
};
