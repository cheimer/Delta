// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/DataObject/ListDataObjectBase.h"

#include "GameUserSettings/FrontGameUserSettings.h"

void UListDataObjectBase::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObjectBase::OnDataObjectInitialized()
{
	
}

void UListDataObjectBase::NotifyListDataModified(UListDataObjectBase* ModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UFrontGameUserSettings::Get()->ApplySettings(true);
		
		bShouldApplyChangeImmediately = false;
	}
}

void UListDataObjectBase::OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EDeltaOptionsListDataModifyReasonType ModifyReason)
{
	OnDependencyDataModified.Broadcast(ModifiedDependencyData, ModifyReason);
}

void UListDataObjectBase::AddEditCondition(const FOptionsDataEditCondition& InEditCondition)
{
	EditConditions.Add(InEditCondition);
}

void UListDataObjectBase::AddEditDependencyData(UListDataObjectBase* InEditDependencyData)
{
	if (!InEditDependencyData || InEditDependencyData->OnListDataModified.IsBoundToObject(this)) return;

	InEditDependencyData->OnListDataModified.AddUObject(this, &ThisClass::OnEditDependencyDataModified);
}

bool UListDataObjectBase::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;

	if (EditConditions.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;
	
	for (const FOptionsDataEditCondition& Condition : EditConditions)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet()) continue;

		bIsEditable = false;

		CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());
		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetForcedStringValue();

			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}

	return bIsEditable;
}
