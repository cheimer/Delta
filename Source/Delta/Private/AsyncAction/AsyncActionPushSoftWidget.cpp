// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction/AsyncActionPushSoftWidget.h"

#include "Subsystem/FrontUISubsystem.h"
#include "UI/ActivatableBaseWidget.h"

void UAsyncActionPushSoftWidget::Activate()
{
	Super::Activate();

	UFrontUISubsystem* FrontUISubsystem = UFrontUISubsystem::Get(CachedOwningWorld.Get());

	FrontUISubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass,
		[this] (EAsyncPushWidgetState InPushState, UActivatableBaseWidget* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
				OnBeforePushSoftWidget.Broadcast(PushedWidget);
				break;
				
			case EAsyncPushWidgetState::AfterPush:
				OnAfterPushSoftWidget.Broadcast(PushedWidget);
				
				if (bCachedFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}

				SetReadyToDestroy();
				break;
			}
		}
	);
}

UAsyncActionPushSoftWidget* UAsyncActionPushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController,
                                                                       TSoftClassPtr<UActivatableBaseWidget> InSoftWidgetClass, UPARAM(meta = (Categories = "Front.WidgetSTack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
	check(!InSoftWidgetClass.IsNull());

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncActionPushSoftWidget* Node = NewObject<UAsyncActionPushSoftWidget>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

			Node->RegisterWithGameInstance(World);
			return Node;
		}
	}

	return nullptr;
}
