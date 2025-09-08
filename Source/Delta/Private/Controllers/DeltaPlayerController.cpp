// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DeltaPlayerController.h"

#include "Characters/DeltaPlayableCharacter.h"
#include "Components/SlateWrapperTypes.h"
#include "UI/DeltaHUD.h"

void ADeltaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ADeltaPlayableCharacter* PlayerCharacter = Cast<ADeltaPlayableCharacter>(GetPawn()))
	{
		PlayerCharacter->OnChangeSkillSet.AddDynamic(this, &ThisClass::HandleChangeSkillSet);
		PlayerCharacter->OnSelectSkill.AddDynamic(this, &ThisClass::HandleSelectSkill);
	}

	DeltaHUD = Cast<ADeltaHUD>(GetHUD());
}

void ADeltaPlayerController::HandleChangeSkillSet(int BeforeIndex, int AfterIndex)
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->ChangeSkillSet(BeforeIndex, AfterIndex);
	
}

void ADeltaPlayerController::HandleSelectSkill(int SelectSet, int SelectIndex, bool bIsSelect)
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->SelectSkill(SelectSet, SelectIndex, bIsSelect);
	
}

void ADeltaPlayerController::TargetDetected(const AActor* Target)
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->ShowTarget(Target);
	
}

void ADeltaPlayerController::TargetLost()
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->HideTarget();
	
}

TArray<UTexture2D*>& ADeltaPlayerController::GetSkillTextures(int Index)
{
	if (ADeltaPlayableCharacter* PlayerCharacter = Cast<ADeltaPlayableCharacter>(GetPawn()))
	{
		return PlayerCharacter->GetSkillTextures(Index);
	}
	else
	{
		static TArray<UTexture2D*> EmptyArray;
		return EmptyArray;
	}
	
}

TArray<int32> ADeltaPlayerController::GetSkillCosts(int Index)
{
	if (ADeltaPlayableCharacter* PlayerCharacter = Cast<ADeltaPlayableCharacter>(GetPawn()))
	{
		return PlayerCharacter->GetSkillCosts(Index);
	}
	else
	{
		return TArray<int32>();
	}
}

void ADeltaPlayerController::SetHudVisible(const bool bIsVisible)
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->SetHudVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
}
