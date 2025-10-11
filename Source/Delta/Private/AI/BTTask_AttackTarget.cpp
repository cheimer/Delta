// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AttackTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/DeltaAIController.h"
#include "GameFramework/Character.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName = "Attack Target Actor";

}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (!TargetObject) return EBTNodeResult::Failed;

	ADeltaAIController* OwnerAIController = Cast<ADeltaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerAIController) return EBTNodeResult::Failed;
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerAIController->GetPawn());
	if (!OwnerCharacter) return EBTNodeResult::Failed;

	UAnimInstance* OwnerAnimIns = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
	if (!OwnerAnimIns) return EBTNodeResult::Failed;

	if (OwnerAnimIns->IsAnyMontagePlaying()) return EBTNodeResult::Failed;
	
	CurrentOwnerComp = &OwnerComp;

	OwnerAIController->AttackTarget();
	OwnerAnimIns->OnMontageEnded.AddDynamic(this, &ThisClass::HandleCurrentMontageEnd);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_AttackTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	CleanUpTask();
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_AttackTarget::HandleCurrentMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	const EBTNodeResult::Type Result = CurrentOwnerComp && !bInterrupted ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	
	FinishLatentTask(*CurrentOwnerComp, Result);
	
	CleanUpTask();
}

void UBTTask_AttackTarget::CleanUpTask()
{
	if (!CurrentOwnerComp) return;
	
	ACharacter* OwnerCharacter = CurrentOwnerComp->GetAIOwner() ? Cast<ACharacter>(CurrentOwnerComp->GetAIOwner()->GetPawn()) : nullptr;
	if (!OwnerCharacter) return;

	UAnimInstance* OwnerAnimIns = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
	if (!OwnerAnimIns) return;

	OwnerAnimIns->OnMontageEnded.RemoveDynamic(this, &ThisClass::HandleCurrentMontageEnd);
	CurrentOwnerComp = nullptr;
}
