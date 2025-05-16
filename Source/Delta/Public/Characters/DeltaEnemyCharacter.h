// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DeltaBaseCharacter.h"
#include "DeltaEnemyCharacter.generated.h"

class ADeltaAIController;
/**
 * 
 */
UCLASS()
class DELTA_API ADeltaEnemyCharacter : public ADeltaBaseCharacter
{
	GENERATED_BODY()

public:
	ADeltaEnemyCharacter();

	void BeginSkillAnim();

protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void CharacterDeath() override;

	UFUNCTION()
	void EndSkillAnim(UAnimMontage* AnimMontage, bool bInterrupted);

private:
	UPROPERTY()
	ADeltaAIController* DeltaAIController;
};
