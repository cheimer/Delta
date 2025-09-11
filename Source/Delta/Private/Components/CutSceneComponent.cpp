// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CutSceneComponent.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Characters/DeltaBaseCharacter.h"
#include "Controllers/DeltaPlayerController.h"

UCutSceneComponent::UCutSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCutSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ADeltaBaseCharacter* OwnerCharacter = Cast<ADeltaBaseCharacter>(GetOwner()))
	{
		OwnerCharacter->OnCharacterDeath.AddUniqueDynamic(this, &ThisClass::OnOwnerDeath);
	}
}

void UCutSceneComponent::OnOwnerDeath(AActor* DeathActor)
{
	StartCutScene(ECutSceneType::Death);
}

void UCutSceneComponent::StartCutScene(const ECutSceneType SceneType)
{
	ULevelSequence* CurrentSequence = nullptr;
	
	if (SceneType == ECutSceneType::Death && IsValid(DeathSequence))
	{
		CurrentSequence = DeathSequence;
	}

	if (CurrentSequence)
	{
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(), CurrentSequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);

		if (GetWorld())
		{
			if (ADeltaPlayerController* PlayerController = Cast<ADeltaPlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				if (bHideHUD)
				{
					PlayerController->SetHudVisible(false);
				}

				if (bDisableInput)
				{
					PlayerController->SetInputEnable(false);
				}
			}
		}

		SequencePlayer->Play();
		SequencePlayer->OnFinished.AddUniqueDynamic(this, &ThisClass::EndCutScene);
	}
}

void UCutSceneComponent::EndCutScene()
{
	if (SequencePlayer)
	{
		SequencePlayer->OnFinished.RemoveDynamic(this, &ThisClass::EndCutScene);
	}

	if (ADeltaPlayerController* PlayerController = Cast<ADeltaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (bHideHUD)
		{
			PlayerController->SetHudVisible(true);
		}
		
		if (bDisableInput)
		{
			PlayerController->SetInputEnable(true);
		}
	}
	
}
