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

	TArray<TWeakObjectPtr<ADeltaBaseCharacter>> PlayableCharacters;
	TArray<TWeakObjectPtr<ADeltaBaseCharacter>> EnemyCharacters;
	
};
