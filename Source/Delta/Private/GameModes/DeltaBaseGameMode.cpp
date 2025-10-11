// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DeltaBaseGameMode.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Characters/Enemy/DeltaEnemyCharacter.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "Controllers/DeltaPlayerController.h"
#include "Helper/DeltaDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/SaveGameSubsystem.h"
#include "UI/DeltaHUD.h"

ADeltaBaseGameMode::ADeltaBaseGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultPawnClass = ADeltaBaseCharacter::StaticClass();
	HUDClass = ADeltaHUD::StaticClass();
	PlayerControllerClass = ADeltaPlayerController::StaticClass();

	CurrentState = EGameModeState::WaitForStart;
	GameStartTime = 0.0f;
}

void ADeltaBaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void ADeltaBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) return;

	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(this, ADeltaBaseCharacter::StaticClass(), TempActors);
	for (AActor* TempActor : TempActors)
	{
		if (ADeltaPlayableCharacter* PlayableCharacter = Cast<ADeltaPlayableCharacter>(TempActor))
		{
			PlayableCharacters.Add(PlayableCharacter);
			PlayableCharacter->OnCharacterDeath.AddDynamic(this, &ThisClass::HandlePlayableCharacterDeath);
		}
		else if (ADeltaEnemyCharacter* EnemyCharacter = Cast<ADeltaEnemyCharacter>(TempActor))
		{
			EnemyCharacters.Add(EnemyCharacter);
			EnemyCharacter->OnCharacterDeath.AddDynamic(this, &ThisClass::HandleEnemyCharacterDeath);
		}
	}
}

void ADeltaBaseGameMode::GameStart()
{
	bIsStarted = true;
	GameStartTime = GetWorld()->GetTimeSeconds();

	CurrentState = EGameModeState::Playing;

	if (USaveGameSubsystem* SaveGameSubsystem = USaveGameSubsystem::Get(GetWorld()))
	{
		SaveGameSubsystem->LoadGame();
	}

}

void ADeltaBaseGameMode::GameEnd()
{
	check(GetWorld());
	
	if (ADeltaPlayerController* PlayerController = Cast<ADeltaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->GameEnd();
	}
}

void ADeltaBaseGameMode::HandleEnemyCharacterDeath(AActor* DeathEnemy)
{
	check(DeathEnemy);
	
	ADeltaEnemyCharacter* CachedDeathEnemy = Cast<ADeltaEnemyCharacter>(DeathEnemy);
	if (!EnemyCharacters.Contains(CachedDeathEnemy))
	{
		DeltaDebug::Print(DeathEnemy->GetName() + " cannot find in GameMode");
		return;
	}

	EnemyCharacters.Remove(CachedDeathEnemy);
	
	if (EnemyCharacters.Num() == 0)
	{
		CurrentState = EGameModeState::Result;
		bIsWin = true;
		GameEnd();
	}
	
}

void ADeltaBaseGameMode::HandlePlayableCharacterDeath(AActor* DeathPlayable)
{
	check(DeathPlayable);
	
	ADeltaPlayableCharacter* CachedDeathPlayable = Cast<ADeltaPlayableCharacter>(DeathPlayable);
	if (!PlayableCharacters.Contains(CachedDeathPlayable))
	{
		DeltaDebug::Print(DeathPlayable->GetName() + " cannot find in GameMode");
		return;
	}

	PlayableCharacters.Remove(CachedDeathPlayable);

	bIsWin = false;
	CurrentState = EGameModeState::Result;
	GameEnd();
	
}

float ADeltaBaseGameMode::GetPlayingTime()
{
	return GetWorld()->GetTimeSeconds() - GameStartTime;
}

void ADeltaBaseGameMode::SetPlayingTime(float NewPlayingTime)
{
	GameStartTime = GetWorld()->GetTimeSeconds() - NewPlayingTime;
}

bool ADeltaBaseGameMode::IsPlayerWin()
{
	return bIsWin;
}
