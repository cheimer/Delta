// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_TargetLocationUpdate.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_TargetLocationUpdate::UBTService_TargetLocationUpdate()
{
	NodeName = "Update Target Location";
}

void UBTService_TargetLocationUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetBlackboardComponent()) return;
	
 	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (!TargetObject) return;

	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Update Target Location2"));

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetActor->GetActorLocation());
}
