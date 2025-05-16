// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillLeftClaws.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API USkillLeftClaws : public USkillBase
{
	GENERATED_BODY()

public:
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

private:
	void AttackTarget();
	
	UPROPERTY(EditAnywhere, Category = "Name", meta = (AllowPrivateAccess = "true"))
	FName SocketName = "LeftClawsSocket";

	UPROPERTY(EditAnywhere, Category = "Values", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.0f;

	UPROPERTY()
	ADeltaBaseCharacter* OwnerDeltaCharacter;

};
