// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DeltaAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Characters/Enemy/DeltaEnemyCharacter.h"
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
		if (!DeltaOwnerCharacter.IsValid()) return;
		
		RunBehaviorTree(BehaviorTree);
	}

	SpawnLocation = InPawn->GetActorLocation();
}

AActor* ADeltaAIController::GetRandPlayableTarget(const float MaxDistance) 
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

	if (!GetPawn()) return nullptr;
	
	TArray<ADeltaBaseCharacter*> TempArray;
	for (ADeltaBaseCharacter* CachedActor : PlayableTargets)
	{
		if (!CachedActor) continue;

		if (FVector::Distance(CachedActor->GetActorLocation(), GetPawn()->GetActorLocation()) < MaxDistance)
		{
			TempArray.Add(CachedActor);
		}
	}
	if (TempArray.IsEmpty()) return nullptr;

	int RandIndex = FMath::RandRange(0,TempArray.Num() - 1);

	CurrentPlayableTarget = PlayableTargets[RandIndex];
	return CurrentPlayableTarget.Get();
}

void ADeltaAIController::SetCurrentSkill()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaEnemyCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return;

	DeltaOwnerCharacter->SetCurrentSkill();

}

TOptional<float> ADeltaAIController::GetCurrentSkillRange()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaEnemyCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return TOptional<float>();

	return DeltaOwnerCharacter->GetCurrentSkillRange();
}

void ADeltaAIController::AttackTarget()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaEnemyCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return;

	DeltaOwnerCharacter->PlaySkillAnimation(DeltaOwnerCharacter->GetCurrentSkill());
}
