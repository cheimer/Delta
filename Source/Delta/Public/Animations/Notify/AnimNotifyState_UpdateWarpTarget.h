// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "AnimNotifyState_UpdateWarpTarget.generated.h"

class ADeltaBaseCharacter;
/**
 * 
 */
UCLASS()
class DELTA_API UAnimNotifyState_UpdateWarpTarget : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	
private:
	TWeakObjectPtr<ADeltaBaseCharacter> OwnerCharacter;

};
