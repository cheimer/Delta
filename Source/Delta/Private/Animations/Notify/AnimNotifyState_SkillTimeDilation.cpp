// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notify/AnimNotifyState_SkillTimeDilation.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "Controllers/DeltaPlayerController.h"
#include "GameModes/DeltaBaseGameMode.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_SkillTimeDilation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(MeshComp->GetOwner());
	if (!DeltaCharacter) return;

	if (IsPlayerRelated(DeltaCharacter))
	{
		DeltaCharacter->BeginAttackDilation(TotalDuration, TimeDilation);
	}
	
}

void UAnimNotifyState_SkillTimeDilation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ADeltaBaseCharacter* DeltaCharacter = Cast<ADeltaBaseCharacter>(MeshComp->GetOwner());
	if (!DeltaCharacter) return;

	/**
	 * don't use IsPlayerRelated(). if Character change during NotifyState, it makes WorldDilation forever.
	 */
	DeltaCharacter->EndAttackDilation();
	
}

bool UAnimNotifyState_SkillTimeDilation::IsPlayerRelated(ADeltaBaseCharacter* OccurActor)
{
	if (!IsValid(OccurActor)) return false;
	
	if (ADeltaPlayableCharacter* OccurPlayer = Cast<ADeltaPlayableCharacter>(OccurActor))
	{
		return OccurPlayer->GetIsPlayerControlled();
	}
	else if (ADeltaPlayableCharacter* TargetPlayer = Cast<ADeltaPlayableCharacter>(OccurActor->GetCurrentSkillTarget()))
	{
		return TargetPlayer->GetIsPlayerControlled();
	}

	return false;
}
