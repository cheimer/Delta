// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ActivatableBaseWidget.h"

#include "Controllers/DeltaFrontPlayerController.h"

ADeltaFrontPlayerController* UActivatableBaseWidget::GetFrontPlayerController()
{
	CachedOwningFrontPC = CachedOwningFrontPC.IsValid() ? CachedOwningFrontPC.Get() : GetOwningPlayer<ADeltaFrontPlayerController>();

	return CachedOwningFrontPC.IsValid() ? CachedOwningFrontPC.Get() : nullptr;
}
