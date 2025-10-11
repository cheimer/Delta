// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SaveGameSubsystem.h"

#include "Helper/DeltaDebugHelper.h"
#include "Interfaces/SaveGameInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/DeltaSaveGame.h"

USaveGameSubsystem* USaveGameSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* FoundWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return UGameInstance::GetSubsystem<USaveGameSubsystem>(FoundWorld->GetGameInstance());
		}
	}

	return nullptr;
}

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SaveSlotString = TEXT("SaveSlotString");
	SaveUserIndex = 0;
	
}

bool USaveGameSubsystem::ShouldCreateSubsystem(UObject* Outer) const
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

bool USaveGameSubsystem::SaveGame()
{
	CurrentSaveGame = CurrentSaveGame ? CurrentSaveGame : Cast<UDeltaSaveGame>(UGameplayStatics::CreateSaveGameObject(UDeltaSaveGame::StaticClass()));
	if (!CurrentSaveGame) return false;

	if (!GetWorld()) return false;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ISaveGameInterface::Execute_SaveData(FoundActor, CurrentSaveGame);
	}

	bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotString, SaveUserIndex);

	FString SaveMessage = bSaveSuccess ? TEXT("Save Success") : TEXT("Save Fail. Check save file");
	DeltaDebug::Print(SaveMessage);

	return bSaveSuccess;
}

bool USaveGameSubsystem::LoadGame()
{
	if (!HaveSaveFile())
	{
		DeltaDebug::Print(TEXT("Cannot find save file"));
		return false;
	}

	CurrentSaveGame = Cast<UDeltaSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotString, SaveUserIndex));
	if (!CurrentSaveGame) return false;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ISaveGameInterface::Execute_LoadData(FoundActor, CurrentSaveGame);
	}

	return true;
}

bool USaveGameSubsystem::HaveSaveFile() const
{
	return UGameplayStatics::DoesSaveGameExist(SaveSlotString, SaveUserIndex);
}

FText USaveGameSubsystem::GetSaveDate() const
{
	if (CurrentSaveGame)
	{
		return FText::FromString(CurrentSaveGame->SavedTime.ToString());
	}

	return FText();
}
