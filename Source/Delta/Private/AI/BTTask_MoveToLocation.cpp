// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PawnMovementComponent.h"

UBTTask_MoveToLocation::UBTTask_MoveToLocation()
{
	NodeName = "Move To Location";
	
	bNotifyTick = true;
	bNotifyTaskFinished = true;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

uint16 UBTTask_MoveToLocation::GetInstanceMemorySize() const
{
	return sizeof(FMoveInfoTaskMemory);
}

EBTNodeResult::Type UBTTask_MoveToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->GetPawn()) return EBTNodeResult::Failed;
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!OwnerPawn->GetMovementComponent()) return EBTNodeResult::Failed;

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (!TargetObject) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor) return EBTNodeResult::Failed;

	FMoveInfoTaskMemory* TaskMemory = CastInstanceNodeMemory<FMoveInfoTaskMemory>(NodeMemory);
	check(TaskMemory);

	TaskMemory->OwningPawn = OwnerPawn;
	TaskMemory->TargetActor = TargetActor;
	
	TaskMemory->DestinationLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(LocationKey.SelectedKeyName);
	TaskMemory->RelativeOffset = TaskMemory->DestinationLocation - TargetActor->GetActorLocation();
	
	TaskMemory->LastUpdateTime = GetWorld()->GetTimeSeconds();
	
	return EBTNodeResult::InProgress;
	
}

void UBTTask_MoveToLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FMoveInfoTaskMemory* TaskMemory = CastInstanceNodeMemory<FMoveInfoTaskMemory>(NodeMemory);
	if (!TaskMemory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (GetWorld()->GetTimeSeconds() - TaskMemory->LastUpdateTime > TaskMemory->UpdateInterval)
	{
		TaskMemory->LastUpdateTime = GetWorld()->GetTimeSeconds();

		FVector CurrentTargetLocation = TaskMemory->TargetActor->GetActorLocation();
		TaskMemory->DestinationLocation = TaskMemory->RelativeOffset + CurrentTargetLocation;
	}

	FVector OwnerLocation = TaskMemory->OwningPawn->GetActorLocation();
	if (FVector2D::Distance(FVector2D(OwnerLocation), FVector2D(TaskMemory->DestinationLocation)) < AcceptRadius)
	{
		TaskMemory->OwningPawn->GetMovementComponent()->Velocity = FVector::ZeroVector;
		TaskMemory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
    
	FVector Direction = (TaskMemory->DestinationLocation - TaskMemory->OwningPawn->GetActorLocation()).GetSafeNormal2D();
	TaskMemory->OwningPawn->AddMovementInput(Direction);
	
}
