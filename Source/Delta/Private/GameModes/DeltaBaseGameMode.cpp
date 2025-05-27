// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DeltaBaseGameMode.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Characters/DeltaEnemyCharacter.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "Kismet/GameplayStatics.h"

class ADeltaPlayableCharacter;

ADeltaBaseGameMode::ADeltaBaseGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CurrentState = EGameModeState::WaitForStart;
}

void ADeltaBaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	/*
	 *	Set Skill, Enemy ...
	 */
}

void ADeltaBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) return;

	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(this, ADeltaBaseCharacter::StaticClass(), TempActors);
	for (AActor* TempActor : TempActors)
	{
		if (ADeltaBaseCharacter* CachedCharacter = Cast<ADeltaBaseCharacter>(TempActor))
		{
			Characters.Add(CachedCharacter);
			CachedCharacter->OnCharacterDeath.AddDynamic(this, &ThisClass::OnCharacterDeath);
		}
	}
}

void ADeltaBaseGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeltaBaseGameMode::GameStart()
{
	bIsStarted = true;
	GameStartTime = GetWorld()->GetTimeSeconds();

	CurrentState = EGameModeState::Playing;
}

void ADeltaBaseGameMode::OnCharacterDeath(AActor* DeathActor)
{
	ADeltaEnemyCharacter* EnemyCharacter = Cast<ADeltaEnemyCharacter>(DeathActor);
	if (EnemyCharacter)
	{
		EnemyCharacterDeath(EnemyCharacter);
	}
	
	ADeltaPlayableCharacter* PlayableCharacter = Cast<ADeltaPlayableCharacter>(DeathActor);
	if (PlayableCharacter)
	{
		PlayableCharacterDeath(PlayableCharacter);
	}
}

void ADeltaBaseGameMode::EnemyCharacterDeath(ADeltaEnemyCharacter* DeathEnemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Death"));
	
	FTimerHandle DelayEnemyDeathTimer;
	GetWorldTimerManager().SetTimer(DelayEnemyDeathTimer, this, &ThisClass::FinishEnemyDeath, 5.0f);
}

void ADeltaBaseGameMode::PlayableCharacterDeath(ADeltaPlayableCharacter* DeathPlayable)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Death"));

	
}

void ADeltaBaseGameMode::FinishEnemyDeath()
{
	
}
