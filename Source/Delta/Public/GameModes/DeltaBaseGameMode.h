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
		
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void HandleCharacterDeath(AActor* DeathActor);
	void EnemyCharacterDeath(ADeltaEnemyCharacter* DeathEnemy);
	void PlayableCharacterDeath(ADeltaPlayableCharacter* DeathPlayable);

	void FinishEnemyDeath();

	EGameModeState CurrentState;
	float GameStartTime;
	bool bIsStarted = false;

	TArray<TWeakObjectPtr<ADeltaBaseCharacter>> Characters;
	
};
