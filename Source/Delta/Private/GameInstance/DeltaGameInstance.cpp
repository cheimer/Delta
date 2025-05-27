// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/DeltaGameInstance.h"

#include "Kismet/GameplayStatics.h"

UDeltaGameInstance::UDeltaGameInstance()
{
	MasterVolume = 1.0f;
	bIsFullscreen = true;
}

void UDeltaGameInstance::Init()
{
	Super::Init();

	LoadFromFile();
	ApplySettings();
}

void UDeltaGameInstance::OnStart()
{
	Super::OnStart();
}

void UDeltaGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UDeltaGameInstance::ApplySettings()
{
	// Set MasterVolume, bIsFullScreen
}

void UDeltaGameInstance::LoadFromFile()
{
	
	
}

void UDeltaGameInstance::SaveToFile()
{
	
}
