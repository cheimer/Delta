// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetLocationFromTarget.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Characters/DeltaBaseCharacter.h"

UBTTask_SetLocationFromTarget::UBTTask_SetLocationFromTarget()
{
	NodeName = "Set Rand Loc far from target";
}

EBTNodeResult::Type UBTTask_SetLocationFromTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!IsValid(&OwnerComp)|| !OwnerComp.GetBlackboardComponent() || !OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;
	ADeltaBaseCharacter* OwnerCharacter = Cast<ADeltaBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerCharacter) return EBTNodeResult::Failed;
	
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (!TargetObject) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor) return EBTNodeResult::Failed;

	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	TargetLocation.Z = OwnerLocation.Z;

	if (!OwnerCharacter->GetCurrentSkillRange().IsSet()) return EBTNodeResult::Failed;
	
	float DistFromTarget = LengthKey.IsBoundOrNonZero() ?
		LengthKey.GetValue(OwnerComp) :
		OwnerCharacter->GetCurrentSkillRange().GetValue() * FMath::RandRange(MinMulti.GetValue(OwnerComp), MaxMulti.GetValue(OwnerComp));

	TArray<FVector> DestLocations;
	float AnglePerNum = 360.0f / PointNum.GetValue(OwnerComp);

	for (int i = 0; i < PointNum.GetValue(OwnerComp); i++)
	{
		float Angle = FMath::DegreesToRadians(AnglePerNum * i);
        
		FVector PointOnCircle;
		PointOnCircle.X = TargetLocation.X + (DistFromTarget * FMath::Cos(Angle));
		PointOnCircle.Y = TargetLocation.Y + (DistFromTarget * FMath::Sin(Angle));
		PointOnCircle.Z = TargetLocation.Z;
        
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem)
		{
			FNavLocation NavLocation;
			if (NavSystem->ProjectPointToNavigation(PointOnCircle, NavLocation))
			{
				DestLocations.Add(NavLocation.Location);
			}
		}
	}

	if (DestLocations.IsEmpty()) return EBTNodeResult::Failed;

	DestLocations.Sort([OwnerLocation](const FVector& A, const FVector& B) -> bool
	{
		return FVector::Distance(OwnerLocation, A) < FVector::Distance(OwnerLocation, B);
	});

	int RandSelect = FMath::RandRange(0, FMath::Min(4, DestLocations.Num()));
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestLocationKey.SelectedKeyName, DestLocations[RandSelect]);

	return EBTNodeResult::Succeeded;
}
