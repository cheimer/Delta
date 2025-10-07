// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/OptionsDataInteractionHelper.h"

#include "GameUserSettings/FrontGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	CachedWeakGameUserSettings = UFrontGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(CachedWeakGameUserSettings.Get(), CachedDynamicFunctionPath, OutStringValue);
	
	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(CachedWeakGameUserSettings.Get(), CachedDynamicFunctionPath, InStringValue);
}
