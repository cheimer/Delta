// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/DeltaAllyCharacter.h"
#include "Controllers/DeltaAllyController.h"
#include "Controllers/DeltaPlayerController.h"
#include "Characters/Enemy/DeltaEnemyCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

ADeltaAllyCharacter::ADeltaAllyCharacter()
{
	// Set default team affiliation for allies (same team as player)
	TeamAffiliation = ETeamAffiliation::Player;

	// Allies can be controlled by AI or player
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ADeltaAllyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADeltaAllyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController)
	{
		// Store reference if this is an AI controller
		AllyAIController = Cast<ADeltaAllyController>(NewController);

		// Check if possessed by player controller
		if (Cast<ADeltaPlayerController>(NewController))
		{
			bIsPlayerControlled = true;
		}
		else
		{
			bIsPlayerControlled = false;
		}
	}
}

void ADeltaAllyCharacter::UnPossessed()
{
	Super::UnPossessed();

	bIsPlayerControlled = false;
}

void ADeltaAllyCharacter::UpdateSkillTarget()
{
	Super::UpdateSkillTarget();

	// Find nearest enemy target for ally AI
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeltaEnemyCharacter::StaticClass(), FoundEnemies);

	ADeltaBaseCharacter* NearestEnemy = nullptr;
	float NearestDistance = FLT_MAX;

	for (AActor* Actor : FoundEnemies)
	{
		ADeltaBaseCharacter* Enemy = Cast<ADeltaBaseCharacter>(Actor);
		if (Enemy && !Enemy->GetIsDead())
		{
			// Check if this is an opponent using team affiliation
			if (Enemy->GetTeamAffiliation() != TeamAffiliation)
			{
				float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
				if (Distance < NearestDistance)
				{
					NearestDistance = Distance;
					NearestEnemy = Enemy;
				}
			}
		}
	}

	if (NearestEnemy)
	{
		SetCurrentSkillTarget(NearestEnemy);
		SetSkillTargetLocation(NearestEnemy->GetActorLocation());
	}
	else
	{
		// If no target found, look ahead
		SetCurrentSkillTarget(nullptr);
		FVector ForwardLocation = GetActorLocation() + GetActorForwardVector() * 1000.0f;
		SetSkillTargetLocation(ForwardLocation);
	}
}

void ADeltaAllyCharacter::OnPlayerControlStart()
{
	bIsPlayerControlled = true;

	// Store the previous controller (AI controller)
	PreviousController = GetController();

	// Additional logic when player takes control
	// e.g., show UI elements, change behavior flags, etc.
}

void ADeltaAllyCharacter::OnPlayerControlEnd()
{
	bIsPlayerControlled = false;

	// Return control to AI controller if it exists
	if (PreviousController && PreviousController->IsValidLowLevel())
	{
		PreviousController->Possess(this);
		AllyAIController = Cast<ADeltaAllyController>(PreviousController);
	}

	PreviousController = nullptr;
}
