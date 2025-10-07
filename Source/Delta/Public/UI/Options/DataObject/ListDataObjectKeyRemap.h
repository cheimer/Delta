// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "UI/Options/DataObject/ListDataObjectBase.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObjectKeyRemap.generated.h"

class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;
/**
 * 
 */
UCLASS()
class DELTA_API UListDataObjectKeyRemap : public UListDataObjectBase
{
	GENERATED_BODY()

public:
	static bool IsPossibleToEdit(const FPlayerKeyMapping& KeyMapping);
	
	void InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile,
		ECommonInputType InInputType, const FPlayerKeyMapping& InOwningPlayerKeyMapping);

	FSlateBrush GetIconFromCurrentKey() const;

	void BindNewInputKey(const FKey& InNewKey);

	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

private:
	FPlayerKeyMapping* GetOwningKeyMapping() const;
	
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	ECommonInputType CachedDesiredInputType;

	FName CachedOwningMappingName;

	EPlayerMappableKeySlot CachedOwningMappableKeySlot;

public:
	ECommonInputType GetCachedDesiredInputType() const {return CachedDesiredInputType;}
	
};
