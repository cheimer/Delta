// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DeltaStructTypes.generated.h"

//Template Code: Copy and edit following lines
USTRUCT(BlueprintType)
struct FDeltaTemplateType
{
	GENERATED_BODY()
 
	UPROPERTY()
	int32 TemplateInt = 0;
};

///////////
/// UI	
///////////

USTRUCT(BlueprintType)
struct FOptionsDataEditCondition
{
	GENERATED_BODY()

public:
	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}
		
		return true;
	}

	bool HasForcedStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

private:
	TFunction<bool()> EditConditionFunc;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;

public:
	void SetEditConditionFunc(const TFunction<bool()>& InEditConditionFunc) {EditConditionFunc = InEditConditionFunc;}

	FString GetDisabledRichReason() const {return DisabledRichReason;}
	void SetDisabledRichReason(const FString& InDisabledRichReason) {DisabledRichReason = InDisabledRichReason;}

	FString GetForcedStringValue() const {return DisabledForcedStringValue.GetValue();}
	void SetForcedStringValue(const FString& InForcedValue) {DisabledForcedStringValue = InForcedValue;}
	
};
