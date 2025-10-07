// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/DataObject/ListDataObjectKeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "Helper/DeltaDebugHelper.h"

bool UListDataObjectKeyRemap::IsPossibleToEdit(const FPlayerKeyMapping& KeyMapping)
{
	bool bIsEditableValue = KeyMapping.GetMappingName() != "ID_Move" &&
		KeyMapping.GetMappingName() != "ID_Look" &&
		KeyMapping.GetMappingName() != "ID_Scroll" &&
		KeyMapping.GetMappingName() != "ID_Cancel";
	
	return bIsEditableValue;
}

void UListDataObjectKeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile,
                                               ECommonInputType InInputType, const FPlayerKeyMapping& InOwningPlayerKeyMapping)
{
	CachedOwningInputUserSettings = InOwningInputUserSettings;
	CachedOwningKeyProfile = InKeyProfile;
	CachedDesiredInputType = InInputType;
	CachedOwningMappingName = InOwningPlayerKeyMapping.GetMappingName();
	CachedOwningMappableKeySlot = InOwningPlayerKeyMapping.GetSlot();
}

FSlateBrush UListDataObjectKeyRemap::GetIconFromCurrentKey() const
{
	check(CachedOwningKeyProfile);

	FSlateBrush FoundBrush;

	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedOwningInputUserSettings->GetLocalPlayer());
	check(CommonInputSubsystem);
	
	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		FoundBrush,
		GetOwningKeyMapping()->GetCurrentKey(),
		CachedDesiredInputType,
		CommonInputSubsystem->GetCurrentGamepadName()
	);

	return FoundBrush;
}

void UListDataObjectKeyRemap::BindNewInputKey(const FKey& InNewKey)
{
	check(CachedOwningInputUserSettings);

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = CachedOwningMappableKeySlot;
	KeyArgs.NewKey = InNewKey;

	FGameplayTagContainer Container;
	
	CachedOwningInputUserSettings->MapPlayerKey(KeyArgs, Container);
	CachedOwningInputUserSettings->SaveSettings();

	NotifyListDataModified(this);
}

bool UListDataObjectKeyRemap::HasDefaultValue() const
{
	return GetOwningKeyMapping()->GetDefaultKey().IsValid();
}

bool UListDataObjectKeyRemap::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && GetOwningKeyMapping()->IsCustomized();
}

bool UListDataObjectKeyRemap::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		check(CachedOwningInputUserSettings);
		
		GetOwningKeyMapping()->ResetToDefault();

		CachedOwningInputUserSettings->SaveSettings();

		NotifyListDataModified(this, EDeltaOptionsListDataModifyReasonType::ResetToDefault);

		return true;
	}

	return false;
}

FPlayerKeyMapping* UListDataObjectKeyRemap::GetOwningKeyMapping() const
{
	check(CachedOwningKeyProfile);

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = CachedOwningMappableKeySlot;

	DeltaDebug::Print(KeyArgs.MappingName.ToString());

	return CachedOwningKeyProfile->FindKeyMapping(KeyArgs);
}
