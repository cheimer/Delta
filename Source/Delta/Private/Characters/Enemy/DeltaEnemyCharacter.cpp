// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/DeltaEnemyCharacter.h"

#include "BrainComponent.h"
#include "MotionWarpingComponent.h"
#include "Controllers/DeltaAIController.h"
#include "DataAssets/Skill/SkillDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void ADeltaEnemyCharacter::OnCharacterDeathHandle(AActor* DeathCharacter)
{
	Super::OnCharacterDeathHandle(DeathCharacter);
	
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
