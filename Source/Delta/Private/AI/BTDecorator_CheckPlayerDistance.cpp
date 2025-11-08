// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTDecorator_CheckPlayerDistance.h"
#include "AIController.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "Controllers/DeltaAllyController.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_CheckPlayerDistance::UBTDecorator_CheckPlayerDistance()
{
	NodeName = "Check Distance From Player";
}

bool UBTDecorator_CheckPlayerDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return false;

	// Find player-controlled character
	ADeltaPlayableCharacter* PlayerCharacter = nullptr;

	// Check all playable characters to find the one controlled by player
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(ControlledPawn->GetWorld(), ADeltaPlayableCharacter::StaticClass(), FoundCharacters);

	for (AActor* Actor : FoundCharacters)
	{
		ADeltaPlayableCharacter* PlayableChar = Cast<ADeltaPlayableCharacter>(Actor);
		if (PlayableChar && PlayableChar->IsPlayerControlled())
		{
			PlayerCharacter = PlayableChar;
			break;
		}
	}

	if (!PlayerCharacter) return false;

	// Calculate distance
	float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerCharacter->GetActorLocation());

	// Return based on check type
	if (bCheckIfTooFar)
	{
		// Pass if too far (distance > max)
		return Distance > MaxDistanceFromPlayer;
	}
	else
	{
		// Pass if close enough (distance < max)
		return Distance < MaxDistanceFromPlayer;
	}
}
