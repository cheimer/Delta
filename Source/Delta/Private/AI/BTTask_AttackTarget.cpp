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

	if (!IsValid(&OwnerComp) || !OwnerComp.GetBlackboardComponent()) return EBTNodeResult::Failed;
	
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (!TargetObject) return EBTNodeResult::Failed;

	ADeltaAIController* OwnerAIController = Cast<ADeltaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerAIController) return EBTNodeResult::Failed;
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerAIController->GetPawn());
	if (!OwnerCharacter) return EBTNodeResult::Failed;

	UAnimInstance* OwnerAnimIns = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
	if (!OwnerAnimIns) return EBTNodeResult::Failed;

	if (OwnerAnimIns->IsAnyMontagePlaying()) return EBTNodeResult::Failed;
	
	OwnerAIController->AttackTarget();
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AttackDelayTimeKey.SelectedKeyName, OwnerAIController->GetCurrentSkillDuration());
	
	return EBTNodeResult::Succeeded;
}
