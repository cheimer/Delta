// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeltaBaseWidget.h"

#include "Controllers/DeltaPlayerController.h"

void UDeltaBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = PlayerController ? PlayerController : Cast<ADeltaPlayerController>(GetOwningPlayer());
	
}

void UDeltaBaseWidget::InItWidget(const EDeltaWidgetType InWidgetType)
{
	WidgetType = InWidgetType;
}
