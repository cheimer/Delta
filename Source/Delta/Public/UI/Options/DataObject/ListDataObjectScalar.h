// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonNumericTextBlock.h"
#include "UI/Options/DataObject/ListDataObjectValue.h"
#include "ListDataObjectScalar.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UListDataObjectScalar : public UListDataObjectValue
{
	GENERATED_BODY()

public:
	static FCommonNumberFormattingOptions NoDecimal();
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFracDigit);

	float GetCurrentValue() const;
	void SetCurrentValueFromSlider(float InNewValue);

private:
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	virtual void OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EDeltaOptionsListDataModifyReasonType ModifyReason) override;

	float StringToFloat(const FString& InString) const;
	
	TRange<float> DisplayValueRange = TRange<float>(0.0f, 1.0f);
	TRange<float> OutputValueRange = TRange<float>(0.0f, 1.0f);

	float SliderStepSize = 0.1f;

	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions NumberFormattingOptions;

public:
#pragma region GetSet
	TRange<float> GetDisplayValueRange() const {return DisplayValueRange;}
	void SetDisplayValueRange(const TRange<float>& InDisplayValueRange) {DisplayValueRange = InDisplayValueRange;}
	
	TRange<float> GetOutputValueRange() const {return OutputValueRange;}
	void SetOutputValueRange(const TRange<float>& InOutputValueRange) {OutputValueRange = InOutputValueRange;}
	
	float GetSliderStepSize() const {return SliderStepSize;}
	void SetSliderStepSize(const float InSliderStepSize) {SliderStepSize = InSliderStepSize;}
	
	ECommonNumericType GetDisplayNumericType() const {return DisplayNumericType;}
	void SetDisplayNumericType(const ECommonNumericType InDisplayNumericType) {DisplayNumericType = InDisplayNumericType;}
	
	FCommonNumberFormattingOptions GetNumberFormattingOptions() const {return NumberFormattingOptions;}
	void SetNumberFormattingOptions(const FCommonNumberFormattingOptions& InNumberFormattingOptions) {NumberFormattingOptions = InNumberFormattingOptions;}
	
#pragma endregion GetSet
	
};
