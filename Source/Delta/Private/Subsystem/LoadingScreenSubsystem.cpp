// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/LoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "DeveloperSettings/LoadingScreenDeveloperSettings.h"
#include "GameModes/DeltaBaseGameMode.h"
#include "Helper/DeltaDebugHelper.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "Kismet/GameplayStatics.h"

bool ULoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	bool SuperValue = Super::ShouldCreateSubsystem(Outer);
	
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}
	
	return SuperValue;
}

void ULoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void ULoadingScreenSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	
}

UWorld* ULoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	UWorld* SuperValue = FTickableGameObject::GetTickableGameObjectWorld();

	if (UGameInstance* OwingGameInstance = GetGameInstance())
	{
		return OwingGameInstance->GetWorld();
	}

	return SuperValue;
}

void ULoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType ULoadingScreenSubsystem::GetTickableTickType() const
{
	ETickableTickType SuperValue = FTickableGameObject::GetTickableTickType();

	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}

	return ETickableTickType::Conditional;
}

bool ULoadingScreenSubsystem::IsTickable() const
{
	bool SuperValue = FTickableGameObject::IsTickable();

	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId ULoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULoadingScreenSubsystem, STATGROUP_Tickables);
}

void ULoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance()) return;

	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentlyLoadingMap = true;

	TryUpdateLoadingScreen();

}

void ULoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}

	
	const ULoadingScreenDeveloperSettings* LoadingScreenSettings = GetDefault<ULoadingScreenDeveloperSettings>();
	
	UGameplayStatics::SetGamePaused(LoadedWorld, true);
	if (!LoadingScreenSettings->bShouldLoadingScreenInEditor)
	{
		UGameplayStatics::SetGamePaused(LoadedWorld, false);
		if (LoadedWorld)
		{
			if (ADeltaBaseGameMode* DeltaGameMode = Cast<ADeltaBaseGameMode>(LoadedWorld->GetAuthGameMode()))
			{
				DeltaGameMode->GameStart();
			}
		}
	}
}

void ULoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	if (IsPreLoadScreenActive()) return;
	
	if (ShouldShowLoadingScreen())
	{
		TryDisplayLoadingScreenIfNone();
		
		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}
	else
	{
		TryRemoveLoadingScreen();

		HoldLoadingScreenStartUpTime = -1.0f;
		
		NotifyLoadingScreenVisibilityChanged(false);
		
		SetTickableTickType(ETickableTickType::Never);
	}
}

bool ULoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}

	return false;
}

bool ULoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const ULoadingScreenDeveloperSettings* LoadingScreenSettings = GetDefault<ULoadingScreenDeveloperSettings>();

	if (GEngine && !LoadingScreenSettings->bShouldLoadingScreenInEditor)
	{
		return false;
	}

	if (CheckTheNeedToShodLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;

		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for Texture");

	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const float CurrentTime = FPlatformTime::Seconds();
	if (HoldLoadingScreenStartUpTime < 0.0f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;

	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
	{
		return true;
	}

	return false;
}

bool ULoadingScreenSubsystem::CheckTheNeedToShodLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");
		return true;
	}

	UWorld* OwningWorld = GetGameInstance()->GetWorld();
	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing World");
		return true;
	}

	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World Begin Playing");
		return true;
	}

	if (!OwningWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("Player Controller Setting");
		return true;
	}

	return false;
}

void ULoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	if (CachedCreatedWidget.IsValid())
	{
		return;
	}
	
	const ULoadingScreenDeveloperSettings* LoadingScreenSettings = GetDefault<ULoadingScreenDeveloperSettings>();

	TSubclassOf<UUserWidget> LoadingScreenClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();

	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadingScreenClass, NAME_None);
	check(CreatedWidget);

	CachedCreatedWidget = CreatedWidget->TakeWidget();

	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(CachedCreatedWidget.ToSharedRef(), 1000);

	NotifyLoadingScreenVisibilityChanged(true);
}

void ULoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (UWorld* World = GetGameInstance()->GetWorld())
	{
		UGameplayStatics::SetGamePaused(World, false);
	}
	
	if (!CachedCreatedWidget.IsValid())
	{
		return;
	}

	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedWidget.ToSharedRef());
	CachedCreatedWidget.Reset();

}

void ULoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	for (ULocalPlayer* ExistLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistLocalPlayer) continue;

		if (APlayerController* PC = ExistLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			if (!bIsVisible)
			{
				FInputModeGameOnly InputMode;
				InputMode.SetConsumeCaptureMouseDown(false);
				PC->SetInputMode(InputMode);
				PC->SetShowMouseCursor(false);
			}
			
			if (PC->Implements<ULoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					ILoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					ILoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}
			}
			
			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<ULoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						ILoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						ILoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}
				}
			}
		}
		
		if (UWorld* World = GetGameInstance()->GetWorld())
		{
			if (!bIsVisible)
			{
				UGameplayStatics::SetGamePaused(World, false);
				
				if (ADeltaBaseGameMode* DeltaGameMode = Cast<ADeltaBaseGameMode>(World->GetAuthGameMode()))
				{
					DeltaGameMode->GameStart();
				}
				
			}
		}
		
	}
}
