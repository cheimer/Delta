// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "DeltaBaseWidget.generated.h"

class ADeltaPlayerController;
/**
 * 
 */
UCLASS()
class DELTA_API UDeltaBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void InItWidget(const EDeltaWidgetType InWidgetType);
	
	virtual void OpenWidget() {}
	virtual void CloseWidget() {}

protected:
	virtual void NativeConstruct() override;

	EDeltaWidgetType WidgetType;

	UPROPERTY()
	ADeltaPlayerController* PlayerController;
	
};
