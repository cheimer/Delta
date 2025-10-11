// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/DeltaEnemyCharacter.h"

#include "BrainComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Controllers/DeltaAIController.h"
#include "DataAssets/Skill/SkillDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameUserSettings/FrontGameUserSettings.h"
#include "Helper/DeltaDebugHelper.h"

ADeltaEnemyCharacter::ADeltaEnemyCharacter()
{
	
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

void ADeltaEnemyCharacter::SaveData_Implementation(UDeltaSaveGame* DeltaSaveGame)
{
	Super::SaveData_Implementation(DeltaSaveGame);

	
}

void ADeltaEnemyCharacter::LoadData_Implementation(UDeltaSaveGame* DeltaSaveGame)
{
	Super::LoadData_Implementation(DeltaSaveGame);

	if (!DeltaSaveGame || !CombatComponent || !UFrontGameUserSettings::Get()) return;

	UFrontGameUserSettings* FrontGameUserSetting = UFrontGameUserSettings::Get();
	const FString& GameDifficulty = FrontGameUserSetting->GetCurrentGameDifficulty();
	
	float DamageMultiplier;
	if (GameDifficulty == "Easy")
	{
		DamageMultiplier = 0.5f;
	}
	else if (GameDifficulty == "Normal")
	{
		DamageMultiplier = 1.0f;
	}
	else if (GameDifficulty == "Hard")
	{
		DamageMultiplier = 1.5f;
	}
	else
	{
		DeltaDebug::Print(TEXT("Cannot find difficulty"));
		return;
	}
	
	CombatComponent->SetDamageMultiplier(DamageMultiplier);
}
