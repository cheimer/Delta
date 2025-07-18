// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notify/AnimNotify_EndSkill.h"

#include "Characters/DeltaBaseCharacter.h"

void UAnimNotify_EndSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp) return;

	ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(MeshComp->GetOwner());
	if (!DeltaCharacter) return;

	DeltaCharacter->EndSkillAnimation();
}
