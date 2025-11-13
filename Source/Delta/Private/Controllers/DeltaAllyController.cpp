// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/DeltaAllyController.h"
#include "Controllers/DeltaPlayerController.h"

ADeltaAllyController::ADeltaAllyController()
{
}

ADeltaBaseCharacter* ADeltaAllyController::GetPlayingCharacter()
{
	if (!GetWorld()) return nullptr;
	
	PlayerController = PlayerController.IsValid() ? PlayerController.Get() : Cast<ADeltaPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerController.IsValid()) return nullptr;

	return PlayerController->GetCurrentCharacter();
}
