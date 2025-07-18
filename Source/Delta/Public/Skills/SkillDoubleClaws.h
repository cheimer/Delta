// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCollisionAttack.h"
#include "Skills/SkillBase.h"
#include "SkillDoubleClaws.generated.h"

class UBoxComponent;

/**
 * Need to LeftClaws, RightClaws Add to Character
 */
UCLASS()
class DELTA_API USkillDoubleClaws : public USkillCollisionAttack
{
	GENERATED_BODY()
	
public:
	USkillDoubleClaws();

	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Name", meta = (AllowPrivateAccess = "true"))
	FName SecondCollisionName = "";

private:
	bool bDoFirstClaw = false;

};
