// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_MoveToPlayer.h"
#include "AIController.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

UBTTask_MoveToPlayer::UBTTask_MoveToPlayer()
{
	NodeName = "Move to Player";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	// Find player-controlled character
	ADeltaPlayableCharacter* PlayerCharacter = nullptr;

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

	if (!PlayerCharacter) return EBTNodeResult::Failed;

	// Calculate distance
	float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerCharacter->GetActorLocation());

	// If already close enough, succeed immediately
	if (Distance <= AcceptanceRadius)
	{
		return EBTNodeResult::Succeeded;
	}

	// Adjust movement speed if urgent
	if (bUseUrgentSpeed && Distance > 1000.0f)
	{
		if (ACharacter* Character = Cast<ACharacter>(ControlledPawn))
		{
			if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
			{
				Movement->MaxWalkSpeed = 600.0f; // Faster when far
			}
		}
	}

	// Move to player location
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(
		PlayerCharacter,
		AcceptanceRadius,
		true,  // bStopOnOverlap
		true,  // bUsePathfinding
		false, // bCanStrafe
		nullptr,
		true   // bAllowPartialPath
	);

	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}
