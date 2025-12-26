// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction/AsyncActionPushConfirmScreen.h"

#include "Subsystem/FrontUISubsystem.h"

void UAsyncActionPushConfirmScreen::Activate()
{
	Super::Activate();

	UFrontUISubsystem::Get(CachedOwningWorld.Get())->PushConfirmScreenToModalStackAsync(CachedScreenType, CachedScreenTitle, CachedScreenMsg,
		[this](EDeltaConfirmButtonType ClickedButtonType)
		{
			OnConfirmScreenButtonClicked.Broadcast(ClickedButtonType);

			SetReadyToDestroy();
		});
}

UAsyncActionPushConfirmScreen* UAsyncActionPushConfirmScreen::PushPushConfirmScreen(const UObject* WorldContextObject,
                                                                                    EDeltaConfirmType ScreenType, FText InScreenTitle, FText InScreenMessage)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncActionPushConfirmScreen* Node = NewObject<UAsyncActionPushConfirmScreen>();
			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMsg = InScreenMessage;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}
