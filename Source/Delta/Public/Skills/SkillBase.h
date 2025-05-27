// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

class ADeltaBaseCharacter;
class UCombatComponent;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class DELTA_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "SkillBase")
	EDeltaSkillType SkillType;

	virtual void BeginSkill(UCombatComponent* InCombatComponent);
	virtual void ReactDamaged();
	virtual void EndSkill();

protected:
	TWeakObjectPtr<UCombatComponent> CombatComponent;
	
};
