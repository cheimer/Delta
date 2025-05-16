// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notify/AnimNotifyState_ActiveSkill.h"

#include "Characters/DeltaBaseCharacter.h"

void UAnimNotifyState_ActiveSkill::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp) return;

	ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(MeshComp->GetOwner());
	if (!DeltaCharacter) return;

	DeltaCharacter->ActiveSkill(SkillType);
	
}

void UAnimNotifyState_ActiveSkill::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(MeshComp->GetOwner());
	if (!DeltaCharacter) return;

	DeltaCharacter->DeActiveSkill();
	
}
