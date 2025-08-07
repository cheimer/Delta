// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DeltaPlayerController.h"

#include "Characters/DeltaPlayableCharacter.h"
#include "UI/DeltaHUD.h"

void ADeltaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ADeltaPlayableCharacter* PlayerCharacter = Cast<ADeltaPlayableCharacter>(GetPawn()))
	{
		PlayerCharacter->OnChangeSkillSet.AddDynamic(this, &ThisClass::HandleChangeSkillSet);
	}
}

void ADeltaPlayerController::HandleChangeSkillSet(int BeforeIndex, int AfterIndex)
{
	if (ADeltaHUD* DeltaHUD = Cast<ADeltaHUD>(GetHUD()))
	{
		DeltaHUD->ChangeSkillSet(BeforeIndex, AfterIndex);
	}
}

void ADeltaPlayerController::LockTargetDetected(AActor* Target) const
{
	if (ADeltaHUD* DeltaHUD = Cast<ADeltaHUD>(GetHUD()))
	{
		DeltaHUD->ShowLockTarget(Target);
	}
}

void ADeltaPlayerController::LockTargetLost() const
{
	if (ADeltaHUD* DeltaHUD = Cast<ADeltaHUD>(GetHUD()))
	{
		DeltaHUD->HideLockTarget();
	}
}

TArray<const UTexture2D*>& ADeltaPlayerController::GetSkillTextures(int Index)
{
	if (ADeltaPlayableCharacter* PlayerCharacter = Cast<ADeltaPlayableCharacter>(GetPawn()))
	{
		return PlayerCharacter->GetSkillTextures(Index);
	}
	else
	{
		static TArray<const UTexture2D*> EmptyArray;
		return EmptyArray;
	}
	
}
