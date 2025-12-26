// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/FrontCommonButtonBase.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Subsystem/FrontUISubsystem.h"

void UFrontCommonButtonBase::NativePreConstruct()
{
	Super::NativeConstruct();

	SetButtonText(ButtonDisplayText);
}

void UFrontCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (ButtonCommonTextBlock && GetCurrentTextStyleClass())
	{
		ButtonCommonTextBlock->SetStyle(GetCurrentTextStyleClass());
	}
}

void UFrontCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		UFrontUISubsystem::Get(GetWorld())->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}
}

void UFrontCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	if (!ButtonDescriptionText.IsEmpty())
	{
		UFrontUISubsystem::Get(GetWorld())->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
	}
	
}

void UFrontCommonButtonBase::SetButtonText(FText InText)
{
	if (ButtonCommonTextBlock && !InText.IsEmpty())
	{
		ButtonCommonTextBlock->SetText(bUseUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

FText UFrontCommonButtonBase::GetButtonDisplayText() const
{
	if (ButtonCommonTextBlock)
	{
		return ButtonCommonTextBlock->GetText();
	}

	return FText::GetEmpty();
}

void UFrontCommonButtonBase::SetButtonDisplayImage(const FSlateBrush& InBrush)
{
	if (ButtonCommonLazyImage)
	{
		ButtonCommonLazyImage->SetBrush(InBrush);
	}
}
