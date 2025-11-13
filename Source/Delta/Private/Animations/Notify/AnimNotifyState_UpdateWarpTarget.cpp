// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notify/AnimNotifyState_UpdateWarpTarget.h"

#include "Characters/DeltaBaseCharacter.h"

void UAnimNotifyState_UpdateWarpTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                    float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(MeshComp->GetOwner());
	if (!DeltaCharacter) return;

	DeltaCharacter->UpdateMotionWarpingTarget();
}

void UAnimNotifyState_UpdateWarpTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(MeshComp->GetOwner());
	if (!DeltaCharacter) return;

	DeltaCharacter->UpdateMotionWarpingTarget();
}
