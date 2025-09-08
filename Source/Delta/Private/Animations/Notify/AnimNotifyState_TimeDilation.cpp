// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notify/AnimNotifyState_TimeDilation.h"

#include "Characters/DeltaBaseCharacter.h"
#include "GameModes/DeltaBaseGameMode.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_TimeDilation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(MeshComp->GetOwner());
	if (!DeltaCharacter) return;

	OwnerCharacter = DeltaCharacter;

	bIsStartDilation = OwnerCharacter->BeginAttackDilation(TotalDuration, TimeDilation);
}

void UAnimNotifyState_TimeDilation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!OwnerCharacter.IsValid()) return;

	if (bIsStartDilation)
	{
		OwnerCharacter->EndAttackDilation();
	}

	
}
