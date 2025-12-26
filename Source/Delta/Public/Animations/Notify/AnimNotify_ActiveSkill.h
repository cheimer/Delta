// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "AnimNotify_ActiveSkill.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UAnimNotify_ActiveSkill : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EDeltaSkillType SkillType;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
