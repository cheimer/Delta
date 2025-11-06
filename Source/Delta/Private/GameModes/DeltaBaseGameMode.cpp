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

	// Find all playable characters (player + AI-controlled teammates)
	TArray<AActor*> PlayableActors;
	UGameplayStatics::GetAllActorsOfClass(this, ADeltaPlayableCharacter::StaticClass(), PlayableActors);
	for (AActor* Actor : PlayableActors)
	{
		if (ADeltaPlayableCharacter* PlayableCharacter = Cast<ADeltaPlayableCharacter>(Actor))
		{
			PlayableCharacters.Add(PlayableCharacter);
			PlayableCharacter->OnCharacterDeath.AddDynamic(this, &ThisClass::HandlePlayableCharacterDeath);
		}
	}

	// Find all enemy characters
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(this, ADeltaEnemyCharacter::StaticClass(), EnemyActors);
	for (AActor* Actor : EnemyActors)
	{
		if (ADeltaEnemyCharacter* EnemyCharacter = Cast<ADeltaEnemyCharacter>(Actor))
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

	// Game is lost when all playable characters (all team members) are dead
	if (PlayableCharacters.Num() == 0)
	{
		bIsWin = false;
		CurrentState = EGameModeState::Result;
		GameEnd();
	}
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

void ADeltaBaseGameMode::RegisterTeamMember(ADeltaPlayableCharacter* TeamMember)
{
	if (!TeamMember) return;

	// Don't add duplicates
	if (PlayableCharacters.Contains(TeamMember)) return;

	PlayableCharacters.Add(TeamMember);
	TeamMember->OnCharacterDeath.AddDynamic(this, &ThisClass::HandlePlayableCharacterDeath);

	// Register with player controller for team management
	if (ADeltaPlayerController* PlayerController = Cast<ADeltaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->RegisterTeamMember(TeamMember);
	}
}
