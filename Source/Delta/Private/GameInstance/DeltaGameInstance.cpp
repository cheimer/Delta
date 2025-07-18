// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/DeltaGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SaveGame/SaveGameData.h"
#include "SaveGame/SaveGameSetting.h"

UDeltaGameInstance::UDeltaGameInstance()
{
	SaveDataSlotName = TEXT("DataSlot");
	SaveSettingSlotName = TEXT("SettingSlot");
}

void UDeltaGameInstance::SaveGameData()
{
	if (!CurrentSaveData)
	{
		CreateSaveGameData();
	}
	
	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		if (APawn* PlayerPawn = PlayerController->GetPawn())
		{
			CurrentSaveData->PlayerLocation = PlayerPawn->GetActorLocation();

			UGameplayStatics::SaveGameToSlot(CurrentSaveData, SaveDataSlotName, UserIndex);
		}
	}
}

void UDeltaGameInstance::LoadGameData()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveDataSlotName, UserIndex)) return;

	CurrentSaveData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveDataSlotName, UserIndex));
	if (!CurrentSaveData) return;

	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		if (APawn* PlayerPawn = PlayerController->GetPawn())
		{
			PlayerPawn->SetActorLocation(CurrentSaveData->PlayerLocation);
		}
	}
	
}

void UDeltaGameInstance::CreateSaveGameData()
{
	CurrentSaveData = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
	check(CurrentSaveData);
	
}

void UDeltaGameInstance::SaveGameSetting()
{
	if (!CurrentSaveSetting)
	{
		CreateSaveGameSetting();
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveSetting, SaveSettingSlotName, UserIndex);
	
}

void UDeltaGameInstance::LoadGameSetting()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSettingSlotName, UserIndex)) return;

	CurrentSaveSetting = Cast<USaveGameSetting>(UGameplayStatics::LoadGameFromSlot(SaveSettingSlotName, UserIndex));
	if (!CurrentSaveSetting) return;

}

void UDeltaGameInstance::CreateSaveGameSetting()
{
	CurrentSaveSetting = Cast<USaveGameSetting>(UGameplayStatics::CreateSaveGameObject(USaveGameSetting::StaticClass()));
	check(CurrentSaveSetting);
	
}
