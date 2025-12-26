// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DeltaBaseWidget.h"
#include "ResultWidget.generated.h"

class UCommonRichTextBlock;
class UCommonButtonBase;
class UCommonNumericTextBlock;
/**
 * 
 */
UCLASS()
class DELTA_API UResultWidget : public UDeltaBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void OpenWidget() override;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
#pragma region Bindings
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* ResultCommonRichText;
	
	UPROPERTY(meta = (BindWidget))
	UCommonNumericTextBlock* TimeCommonNumericText;
	
	UPROPERTY(meta = (BindWidget))
	UCommonNumericTextBlock* RemainHealthCommonNumericText;
	
	UPROPERTY(meta = (BindWidget))
	UCommonNumericTextBlock* TotalDamageCommonNumericText;
	
	UPROPERTY(meta = (BindWidget))
	UCommonButtonBase* MainButton;
	
	UPROPERTY(meta = (BindWidget))
	UCommonButtonBase* QuitButton;

#pragma endregion Bindings
};
