// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsDetailWidget.generated.h"

class UListDataObjectBase;
class UCommonRichTextBlock;
class UCommonLazyImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UOptionsDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(UListDataObjectBase* InDataObject, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsViewInfo();

protected:
	void NativeOnInitialized() override;

private:
#pragma region Bindings
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* TitleTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* DescriptionLazyImage;
	
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* DescriptionRichTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* DynamicDetailsRichTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* DisabledReasonRichTextBlock;
		
#pragma endregion Bindings
	
};
