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

	virtual void PlaySkillAnimation(const EDeltaSkillType SkillType) override;
	virtual void EndSkillAnimation() override;

	virtual void UpdateSkillTarget() override;

	virtual void SetCurrentSkill();
	EDeltaSkillType GetCurrentSkill() const;

	void ApplyDifficultySettings();
	
#pragma region ISaveGameInterface
	virtual void SaveData_Implementation(UDeltaSaveGame* DeltaSaveGame) override;
	virtual void LoadData_Implementation(UDeltaSaveGame* DeltaSaveGame) override;

#pragma endregion ISaveGameInterface

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void HandleCharacterDeath(AActor* DeathCharacter) override;

private:
	UPROPERTY()
	ADeltaAIController* DeltaAIController;

};
