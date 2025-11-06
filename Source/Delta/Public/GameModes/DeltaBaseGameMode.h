// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DeltaBaseGameMode.generated.h"

class ADeltaPlayableCharacter;
class ADeltaEnemyCharacter;
class ADeltaAllyCharacter;

enum class EGameModeState
{
	WaitForStart,
	Playing,
	Result
};

class ADeltaBaseCharacter;
/**
 * 
 */
UCLASS()
class DELTA_API ADeltaBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADeltaBaseGameMode();

	void GameStart();

	float GetPlayingTime();
	void SetPlayingTime(float NewPlayingTime);

	bool IsPlayerWin();

	// Register ally character for tracking
	void RegisterAlly(ADeltaAllyCharacter* Ally);
		
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

	void GameEnd();

private:
	UFUNCTION()
	void HandleEnemyCharacterDeath(AActor* DeathEnemy);
	UFUNCTION()
	void HandlePlayableCharacterDeath(AActor* DeathPlayable);
	UFUNCTION()
	void HandleAllyCharacterDeath(AActor* DeathAlly);

	EGameModeState CurrentState;
	float GameStartTime;
	bool bIsStarted = false;
	bool bIsWin = false;

	TArray<TWeakObjectPtr<ADeltaBaseCharacter>> PlayableCharacters;
	TArray<TWeakObjectPtr<ADeltaBaseCharacter>> EnemyCharacters;
	TArray<TWeakObjectPtr<ADeltaBaseCharacter>> AllyCharacters;
};
