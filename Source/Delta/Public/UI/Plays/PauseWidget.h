// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DeltaBaseWidget.h"
#include "PauseWidget.generated.h"

class UCommonNumericTextBlock;
class UCommonButtonBase;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DELTA_API UPauseWidget : public UDeltaBaseWidget
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
	UCommonNumericTextBlock* TimeCommonNumericText;
	
	UPROPERTY(meta = (BindWidget))
	UCommonButtonBase* ContinueButton;
	
	UPROPERTY(meta = (BindWidget))
	UCommonButtonBase* MainButton;
	
	UPROPERTY(meta = (BindWidget))
	UCommonButtonBase* QuitButton;

#pragma endregion Bindings
};
