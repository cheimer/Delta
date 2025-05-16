// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DeltaAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Characters/DeltaEnemyCharacter.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "Kismet/GameplayStatics.h"

ADeltaAIController::ADeltaAIController()
{
}

void ADeltaAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	check(BehaviorTree && BehaviorTree->BlackboardAsset);
	if (BehaviorTree && BehaviorTree->BlackboardAsset)
	{
		DeltaOwnerCharacter = DeltaOwnerCharacter.Get() ? DeltaOwnerCharacter.Get() : Cast<ADeltaEnemyCharacter>(InPawn);
		if (!DeltaOwnerCharacter.Get()) return;
		
		RunBehaviorTree(BehaviorTree);
	}
}

ADeltaBaseCharacter* ADeltaAIController::GetRandPlayableTarget() 
{
	if (PlayableTargets.IsEmpty())
	{
		TArray<AActor*> TempArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeltaPlayableCharacter::StaticClass(), TempArray);

		for (AActor* CachedActor : TempArray)
		{
			if (ADeltaBaseCharacter* PlayableCharacter = Cast<ADeltaBaseCharacter>(CachedActor))
			{
				PlayableTargets.Add(PlayableCharacter);
			}
		}
	}

	if (PlayableTargets.IsEmpty()) return nullptr;

	int RandIndex = FMath::RandRange(0,PlayableTargets.Num() - 1);

	CurrentPlayableTarget = PlayableTargets[RandIndex];
	return CurrentPlayableTarget.Get();
}

void ADeltaAIController::AttackTarget()
{
	if (!CurrentPlayableTarget.Get()) return;

	DeltaOwnerCharacter = DeltaOwnerCharacter.Get() ? DeltaOwnerCharacter.Get() : Cast<ADeltaEnemyCharacter>(GetOwner());
	if (!DeltaOwnerCharacter.Get()) return;

	DeltaOwnerCharacter->BeginSkillAnim();
}
