// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PrimaryLayOutWidget.h"
#include "Helper/DeltaDebugHelper.h"

UCommonActivatableWidgetContainerBase* UPrimaryLayOutWidget::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	check(RegisteredWidgetStackMap.Contains(InTag));

	return RegisteredWidgetStackMap.FindRef(InTag);
	
}

void UPrimaryLayOutWidget::RegisterWidgetStack(UPARAM(meta = (Categories = "Front.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}
