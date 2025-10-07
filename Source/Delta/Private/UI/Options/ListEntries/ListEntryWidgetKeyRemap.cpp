// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/ListEntries/ListEntryWidgetKeyRemap.h"

#include "BlueprintFunctionLibrary/FrontFunctionLibrary.h"
#include "GameplayTags/FrontGameplayTags.h"
#include "Subsystem/FrontUISubsystem.h"
#include "UI/Components/FrontCommonButtonBase.h"
#include "UI/Options/KeyRemapScreenWidget.h"
#include "UI/Options/DataObject/ListDataObjectKeyRemap.h"

void UListEntryWidgetKeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RemapKeyCommonButton->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	ResetKeyCommonButton->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UListEntryWidgetKeyRemap::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObjectKeyRemap>(InOwningListDataObject);

	RemapKeyCommonButton->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UListEntryWidgetKeyRemap::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);

	if (CachedOwningKeyRemapDataObject)
	{
		RemapKeyCommonButton->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
	
}

void UListEntryWidgetKeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();
	
	UFrontUISubsystem::Get(GetWorld())->PushSoftWidgetToStackAsync(
		FrontGameplayTags::Front_WidgetStack_Modal,
		UFrontFunctionLibrary::GetFrontSoftWidgetClassByTag(FrontGameplayTags::Front_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UActivatableBaseWidget* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UKeyRemapScreenWidget* CreatedKeyRemapScreen = CastChecked<UKeyRemapScreenWidget>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyToRemapCanceled);

				if (CachedOwningKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetCachedDesiredInputType());
				}
			}
		}
	);
}

void UListEntryWidgetKeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedOwningKeyRemapDataObject) return;
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UFrontUISubsystem::Get(GetWorld())->PushConfirmScreenToModalStackAsync(
			EDeltaConfirmType::OK,
			FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" is already using" )),
			[](EDeltaConfirmButtonType ClickedButton){}
		);

		return;
	}

	UFrontUISubsystem::Get(GetWorld())->PushConfirmScreenToModalStackAsync(
		EDeltaConfirmType::YesNo,
		FText::FromString(TEXT("Reset Key Mapping")),
		FText::FromString(TEXT("Sure to reset key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString()),
		[this](EDeltaConfirmButtonType ClickedButton)
		{
			if (ClickedButton == EDeltaConfirmButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
			}
		}
	);
	
	
}

void UListEntryWidgetKeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UListEntryWidgetKeyRemap::OnKeyToRemapCanceled(const FString& CanceledReason)
{
	UFrontUISubsystem::Get(GetWorld())->PushConfirmScreenToModalStackAsync(
		EDeltaConfirmType::OK,
		FText::FromString(TEXT("Key Remap")),
		FText::FromString(CanceledReason),
		[](EDeltaConfirmButtonType ClickedButton)
		{
			
		}
	);
}
