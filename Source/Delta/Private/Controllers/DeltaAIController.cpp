// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DeltaAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Characters/DeltaBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

ADeltaAIController::ADeltaAIController()
{
}

void ADeltaAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	check(BehaviorTree && BehaviorTree->BlackboardAsset);
	
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaBaseCharacter>(InPawn);
	if (!DeltaOwnerCharacter.IsValid()) return;
	
	RunBehaviorTree(BehaviorTree);
	
}

AActor* ADeltaAIController::SetRandTarget(const float MaxDistance) 
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaBaseCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return nullptr;
	
	if (Targets.IsEmpty())
	{
		ETeamAffiliation OwnerTeam = DeltaOwnerCharacter->GetTeamAffiliation();
		
		TArray<AActor*> TempArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeltaBaseCharacter::StaticClass(), TempArray);

		for (AActor* CachedActor : TempArray)
		{
			if (ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(CachedActor))
			{
				if (DeltaCharacter->GetTeamAffiliation() != OwnerTeam || DeltaCharacter->GetTeamAffiliation() == ETeamAffiliation::Neutral)
				{
					Targets.Add(DeltaCharacter);
				}
			}
		}
	}

	Targets.RemoveAll([](TWeakObjectPtr<ADeltaBaseCharacter> Val)->bool
	{
		return !Val.IsValid() || Val->GetIsDead();
	});
	
	if (Targets.IsEmpty()) return nullptr;

	TArray<ADeltaBaseCharacter*> TargetsInDistance;
	for (TWeakObjectPtr<ADeltaBaseCharacter> CachedActor : Targets)
	{
		if (!CachedActor.IsValid()) continue;

		if (FVector::Distance(CachedActor->GetActorLocation(), GetPawn()->GetActorLocation()) < MaxDistance)
		{
			TargetsInDistance.Add(CachedActor.Get());
		}
	}
	if (TargetsInDistance.IsEmpty()) return nullptr;

	int RandIndex = FMath::RandRange(0,TargetsInDistance.Num() - 1);

	CurrentTarget = Targets[RandIndex];

	DeltaOwnerCharacter->SetCurrentSkillTarget(CurrentTarget.Get());
	return CurrentTarget.Get();
}

void ADeltaAIController::AttackTarget()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaBaseCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return;

	EDeltaSkillType SkillName = DeltaOwnerCharacter->GetCurrentSkillName();

	DeltaOwnerCharacter->BeginSkill(SkillName);
}

void ADeltaAIController::RotateToTarget()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaBaseCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid() || !CurrentTarget.IsValid()) return;

	FVector Direction = CurrentTarget->GetActorLocation() - DeltaOwnerCharacter->GetActorLocation();
	DeltaOwnerCharacter->SetActorRotation(Direction.Rotation());
}

void ADeltaAIController::SetCurrentSkill()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaBaseCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return;

	DeltaOwnerCharacter->SetCurrentSkill();

}

TOptional<float> ADeltaAIController::GetCurrentSkillRange()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaBaseCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return TOptional<float>();

	return DeltaOwnerCharacter->GetCurrentSkillRange();
}

float ADeltaAIController::GetCurrentSkillDuration()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaBaseCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return 0.0f;

	return DeltaOwnerCharacter->GetSkillDurationTime(DeltaOwnerCharacter->GetCurrentSkillName());
}
