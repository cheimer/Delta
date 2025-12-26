// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/FrontUISubsystem.h"

#include "BlueprintFunctionLibrary/FrontFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "GameplayTags/FrontGameplayTags.h"
#include "Helper/DeltaDebugHelper.h"
#include "UI/ActivatableBaseWidget.h"
#include "UI/ConfirmScreenWidget.h"
#include "UI/PrimaryLayOutWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UFrontUISubsystem* UFrontUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UFrontUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UFrontUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}
	
	return false;
}

void UFrontUISubsystem::RegisterCreatedPrimaryLayout(UPrimaryLayOutWidget* InPrimaryLayOutWidget)
{
	check(InPrimaryLayOutWidget);
	
	CreatedPrimaryLayout = InPrimaryLayOutWidget;
}

void UFrontUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag, TSoftClassPtr<UActivatableBaseWidget> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, UActivatableBaseWidget*)> AsyncPushStateCallback)
{
	 check(!InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, this, WidgetStackTag, AsyncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				check(LoadedWidgetClass && CreatedPrimaryLayout);

				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(WidgetStackTag);

				UActivatableBaseWidget* CreatedWidget = FoundWidgetStack->AddWidget<UActivatableBaseWidget>(
					LoadedWidgetClass,
					[AsyncPushStateCallback](UActivatableBaseWidget& CreatedWidgetInstance)
					{
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
	
}

void UFrontUISubsystem::PushConfirmScreenToModalStackAsync(EDeltaConfirmType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg,
	TFunction<void(EDeltaConfirmButtonType)> ButtonClickedCallback)
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;
	
	switch (InScreenType)
	{
	case EDeltaConfirmType::OK:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenTitle, InScreenMsg);
		break;
	case EDeltaConfirmType::YesNo:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMsg);
		break;
	case EDeltaConfirmType::OKCancel:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(InScreenTitle, InScreenMsg);
		break;
	case EDeltaConfirmType::Unknown:
		break;
	}

	check(CreatedInfoObject);

	PushSoftWidgetToStackAsync(FrontGameplayTags::Front_WidgetStack_Modal,
		UFrontFunctionLibrary::GetFrontSoftWidgetClassByTag(FrontGameplayTags::Front_Widget_ConfirmScreen),
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushState, UActivatableBaseWidget* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UConfirmScreenWidget* CreatedConfirmScreen = CastChecked<UConfirmScreenWidget>(PushedWidget);
				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickedCallback);
			}
		}
	);
	
}
