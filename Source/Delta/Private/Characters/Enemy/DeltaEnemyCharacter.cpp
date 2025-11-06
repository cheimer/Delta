// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/DeltaEnemyCharacter.h"

#include "BrainComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Controllers/DeltaAIController.h"
#include "DataAssets/Skill/SkillDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameUserSettings/FrontGameUserSettings.h"
#include "Helper/DeltaDebugHelper.h"

ADeltaEnemyCharacter::ADeltaEnemyCharacter()
{
	// Set default team affiliation for enemy characters
	TeamAffiliation = ETeamAffiliation::Enemy;
}

void ADeltaEnemyCharacter::BeginPlay()
{
	ApplyDifficultySettings();

	Super::BeginPlay();
	
	
}

void ADeltaEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController)
	{
		DeltaAIController = Cast<ADeltaAIController>(NewController);
	}
	
}

void ADeltaEnemyCharacter::HandleCharacterDeath(AActor* DeathCharacter)
{
	Super::HandleCharacterDeath(DeathCharacter);
	
	DeltaAIController = DeltaAIController ? DeltaAIController : Cast<ADeltaAIController>(GetController());
	
	if (!DeltaAIController || !DeltaAIController->GetBrainComponent()) return;
	DeltaAIController->GetBrainComponent()->Cleanup();
}

void ADeltaEnemyCharacter::PlaySkillAnimation(const EDeltaSkillType SkillType)
{
	if (SkillDataAssets.IsEmpty()) return;

	Super::PlaySkillAnimation(SkillType);

	UpdateSkillTarget();
	UpdateMotionWarpingTarget();
}

void ADeltaEnemyCharacter::EndSkillAnimation()
{
	Super::EndSkillAnimation();
}

void ADeltaEnemyCharacter::UpdateSkillTarget()
{
	Super::UpdateSkillTarget();
	
	DeltaAIController = DeltaAIController ? DeltaAIController : Cast<ADeltaAIController>(GetController());
	if (!DeltaAIController || !DeltaAIController->GetCurrentTarget()) return;

	SetCurrentSkillTarget(DeltaAIController->GetCurrentTarget());
	SetSkillTargetLocation(DeltaAIController->GetCurrentTarget()->GetActorLocation());
}

void ADeltaEnemyCharacter::SetCurrentSkill()
{
	int RandIndex = FMath::RandRange(0, SkillDataAssets.Num() - 1);

	CachedSkillData = SkillDataAssets[RandIndex];
}

EDeltaSkillType ADeltaEnemyCharacter::GetCurrentSkill() const
{
	if (!CachedSkillData.IsValid()) return EDeltaSkillType::Max;

	return CachedSkillData->Type;
}

void ADeltaEnemyCharacter::ApplyDifficultySettings()
{
	UFrontGameUserSettings* UserSettings = UFrontGameUserSettings::Get();
	if (!UserSettings || !HealthComponent || !CombatComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADeltaEnemyCharacter::ApplyDifficultySettings - Missing required components"));
		return;
	}

	const EDeltaGameDifficulty Difficulty = UserSettings->GetCurrentGameDifficulty();

	float HealthMultiplier = 1.0f;
	float DamageMultiplier = 1.0f;

	switch (Difficulty)
	{
	case EDeltaGameDifficulty::Easy:
		HealthMultiplier = 0.75f;
		DamageMultiplier = 0.5f;
		break;

	case EDeltaGameDifficulty::Normal:
		HealthMultiplier = 1.0f;
		DamageMultiplier = 1.0f;
		break;

	case EDeltaGameDifficulty::Hard:
		HealthMultiplier = 1.5f;
		DamageMultiplier = 1.5f;
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("ADeltaEnemyCharacter::ApplyDifficultySettings - Unknown difficulty"));
		break;
	}

	const float BaseMaxHealth = HealthComponent->GetMaxHealth();
	HealthComponent->SetMaxHealth(BaseMaxHealth * HealthMultiplier);
	CombatComponent->SetDamageMultiplier(DamageMultiplier);

}

void ADeltaEnemyCharacter::SaveData_Implementation(UDeltaSaveGame* DeltaSaveGame)
{
	Super::SaveData_Implementation(DeltaSaveGame);
	
}

void ADeltaEnemyCharacter::LoadData_Implementation(UDeltaSaveGame* DeltaSaveGame)
{
	Super::LoadData_Implementation(DeltaSaveGame);

}
