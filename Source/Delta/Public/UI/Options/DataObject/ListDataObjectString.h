// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Options/DataObject/ListDataObjectValue.h"
#include "ListDataObjectString.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UListDataObjectString : public UListDataObjectValue
{
	GENERATED_BODY()

public:
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);

	void AdvanceToNextOption();
	void BackToBeforeOption();

	void OnRotatorInitiatedValueChanged(const FText& InNewSelectedText);

protected:
	virtual void OnDataObjectInitialized() override;
	
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	
	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const override;
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) override;

	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	
	FString CurrentStringValue;
	FText CurrentDisplayText;
	
	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;

public:
	const FText& GetCurrentDisplayText() const {return CurrentDisplayText;}

	const TArray<FString>& GetAvailableOptionsStringArray() const {return AvailableOptionsStringArray;}
	const TArray<FText>& GetAvailableOptionsTextArray() const {return AvailableOptionsTextArray;}
	
};


UCLASS()
class DELTA_API UListDataObjectStringBool : public UListDataObjectString
{
	GENERATED_BODY()

public:
	void OverrideTrueDisplayText(const FText& InNewTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InNewFalseDisplayText);

	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

protected:
	virtual void OnDataObjectInitialized() override;
	
private:
	void TryInitBoolValues();
	
	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");

};

UCLASS()
class DELTA_API UListDataObjectStringEnum : public UListDataObjectString
{
	GENERATED_BODY()

public:
	template<typename EnumType>
	void AddEnumOption(EnumType InEnumOption, const FText& InDisplayText)
	{
		if (!std::is_enum_v<EnumType>) return;
	
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);
		
		AddDynamicOption(ConvertedEnumString, InDisplayText);
	}
	
	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		if (!std::is_enum_v<EnumType>) return EnumType();
	
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();

		//(EnumType)
		return static_cast<EnumType>(StaticEnumOption->GetValueByNameString(CurrentStringValue));
	}
	
	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		if (!std::is_enum_v<EnumType>) return;
	
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		SetDefaultValueFromString(ConvertedEnumString);
	}

};

UCLASS()
class DELTA_API UListDataObjectStringInteger : public UListDataObjectString
{
	GENERATED_BODY()

public:
	void AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText);
	
protected:
	virtual void OnDataObjectInitialized() override;
	
	virtual void OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EDeltaOptionsListDataModifyReasonType ModifyReason) override;
	
};
