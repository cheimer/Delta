// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "Engine/DataAsset.h"
#include "Skills/SkillBase.h"
#include "SkillDataAsset.generated.h"

/**
 * SkillDataAsset work before make SkillBase class.
 * So, Need to set up default like Type, Subclass, Montage...
 * Also before use skill, need provide cost, distance...
 */
UCLASS()
class DELTA_API USkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	EDeltaSkillType Type;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSet<EDeltaSkillState> UsableState;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<USkillBase> Skill;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Cost = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Distance = 0.0f;

};
