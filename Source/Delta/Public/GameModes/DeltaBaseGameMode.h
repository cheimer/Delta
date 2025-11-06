// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DeltaBaseGameMode.generated.h"

class ADeltaPlayableCharacter;
class ADeltaEnemyCharacter;

enum class EGameModeState
{
	WaitForStart,
	Playing,
	Result
};

class ADeltaBaseCharacter;
/**
 * Game mode that manages all playable characters (player + AI-controlled allies)
 * and enemy characters
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

	// Register team member (playable character) for tracking
	void RegisterTeamMember(ADeltaPlayableCharacter* TeamMember);
		
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

	void GameEnd();

private:
	UFUNCTION()
	void HandleEnemyCharacterDeath(AActor* DeathEnemy);
	UFUNCTION()
	void HandlePlayableCharacterDeath(AActor* DeathPlayable);

	EGameModeState CurrentState;
	float GameStartTime;
	bool bIsStarted = false;
	bool bIsWin = false;

	// All playable characters (player + AI-controlled team members)
	TArray<TWeakObjectPtr<ADeltaPlayableCharacter>> PlayableCharacters;
	// All enemy characters
	TArray<TWeakObjectPtr<ADeltaEnemyCharacter>> EnemyCharacters;
};
