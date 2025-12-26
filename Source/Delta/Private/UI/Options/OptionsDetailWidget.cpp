// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/OptionsDetailWidget.h"

#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "UI/Options/DataObject/ListDataObjectBase.h"

void UOptionsDetailWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();
}

void UOptionsDetailWidget::UpdateDetailsViewInfo(UListDataObjectBase* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject) return;

	TitleTextBlock->SetText(InDataObject->GetDataDisplayName());
	
	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		DescriptionLazyImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		DescriptionLazyImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		DescriptionLazyImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	DescriptionRichTextBlock->SetText(InDataObject->GetDescriptionRichText());
	
	const FString DynamicDetails = FString::Printf(TEXT("Data Object Class : <Bold>%s</>\n\nEntry Widget Class : <Bold>%s</>"),
		*InDataObject->GetClass()->GetName(), *InEntryWidgetClassName);
	DynamicDetailsRichTextBlock->SetText(FText::FromString(DynamicDetails));
	
	DisabledReasonRichTextBlock->SetText(InDataObject->IsDataCurrentlyEditable() ? FText::GetEmpty() : InDataObject->GetDisabledRichText());
}

void UOptionsDetailWidget::ClearDetailsViewInfo()
{
	TitleTextBlock->SetText(FText::GetEmpty());
	DescriptionLazyImage->SetVisibility(ESlateVisibility::Collapsed);
	DescriptionRichTextBlock->SetText(FText::GetEmpty());
	DynamicDetailsRichTextBlock->SetText(FText::GetEmpty());
	DisabledReasonRichTextBlock->SetText(FText::GetEmpty());
}
