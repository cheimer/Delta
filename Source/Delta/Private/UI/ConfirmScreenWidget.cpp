// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ConfirmScreenWidget.h"

#include "CommonTextBlock.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"
#include "UI/Components/FrontCommonButtonBase.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();

	if (!InfoObject) return nullptr;

	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmButtonType = EDeltaConfirmButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString("OK");

	InfoObject->AvailableScreenButtons.Add(OKButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();

	if (!InfoObject) return nullptr;

	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmButtonType = EDeltaConfirmButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString("Yes");

	FConfirmButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmButtonType = EDeltaConfirmButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString("No");

	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();

	if (!InfoObject) return nullptr;

	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmButtonType = EDeltaConfirmButtonType::Confirmed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString("OK");

	FConfirmButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmButtonType = EDeltaConfirmButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString("Cancel");

	InfoObject->AvailableScreenButtons.Add(OKButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);
	
	return InfoObject;
}

void UConfirmScreenWidget::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EDeltaConfirmButtonType)> ClickButtonCallback)
{
	check(InScreenInfoObject && TitleTextBlock && MessageTextBlock && ButtonsDynamicEntryBox);

	TitleTextBlock->SetText(InScreenInfoObject->ScreenTitle);
	MessageTextBlock->SetText(InScreenInfoObject->ScreenMessage);

	if (ButtonsDynamicEntryBox->GetNumEntries() != 0)
	{
		ButtonsDynamicEntryBox->Reset<UFrontCommonButtonBase>(
			[](UFrontCommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			}
		);
	}

	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());

	for (const FConfirmButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		UFrontCommonButtonBase* AddedButton = ButtonsDynamicEntryBox->CreateEntry<UFrontCommonButtonBase>();
		if (!AddedButton) continue;
		
		AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
		AddedButton->OnClicked().AddLambda(
			[ClickButtonCallback, AvailableButtonInfo, this]()
			{
				ClickButtonCallback(AvailableButtonInfo.ConfirmButtonType);

				DeactivateWidget();
			}
		);
		
	}
	
}

UWidget* UConfirmScreenWidget::NativeGetDesiredFocusTarget() const
{
	if (ButtonsDynamicEntryBox->GetNumEntries() != 0)
	{
		ButtonsDynamicEntryBox->GetAllEntries().Last()->SetFocus();
	}
	
	return Super::NativeGetDesiredFocusTarget();
	
}
