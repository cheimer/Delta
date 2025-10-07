// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "UI/ActivatableBaseWidget.h"
#include "ConfirmScreenWidget.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDeltaConfirmButtonType ConfirmButtonType = EDeltaConfirmButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class DELTA_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static UConfirmScreenInfoObject* CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	
	UPROPERTY(Transient)
	FText ScreenTitle;
	
	UPROPERTY(Transient)
	FText ScreenMessage;
	
	UPROPERTY(Transient)
	TArray<FConfirmButtonInfo> AvailableScreenButtons;
	
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UConfirmScreenWidget : public UActivatableBaseWidget
{
	GENERATED_BODY()

public:
	void InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EDeltaConfirmButtonType)> ClickButtonCallback);

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* TitleTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* MessageTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* ButtonsDynamicEntryBox;
	
};
