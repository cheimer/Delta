// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontCommonButtonBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DELTA_API UFrontCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	virtual void NativeOnCurrentTextStyleChanged() override;

	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;

	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);
	
	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;
	
	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Front Button", meta = (AllowPrivateAccess = true))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Front Button", meta = (AllowPrivateAccess = true))
	bool bUseUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Front Button", meta = (AllowPrivateAccess = true))
	FText ButtonDescriptionText;

#pragma region Binds
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* ButtonCommonTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonLazyImage* ButtonCommonLazyImage;

#pragma endregion Binds
	
};
