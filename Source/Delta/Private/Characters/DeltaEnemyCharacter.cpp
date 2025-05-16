// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DeltaEnemyCharacter.h"

#include "BrainComponent.h"
#include "MotionWarpingComponent.h"
#include "Controllers/DeltaAIController.h"
#include "DataAssets/Skill/SkillDataAsset.h"

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

void ADeltaEnemyCharacter::CharacterDeath()
{
	Super::CharacterDeath();
	
	DeltaAIController = DeltaAIController ? DeltaAIController : Cast<ADeltaAIController>(GetController());
	
	if (!DeltaAIController || !DeltaAIController->GetBrainComponent()) return;
	DeltaAIController->GetBrainComponent()->Cleanup();
}

void ADeltaEnemyCharacter::BeginSkillAnim()
{
	if (SkillDataAssets.IsEmpty()) return;

	int RandIndex = FMath::RandRange(0, SkillDataAssets.Num() - 1);
	USkillDataAsset* SkillData = FindSkillDataAsset(SkillDataAssets[RandIndex]->Type);
	if (!SkillData) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	
	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &ADeltaEnemyCharacter::EndSkillAnim);

	AnimInstance->Montage_Play(SkillData->AnimMontage);
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, SkillData->AnimMontage);
		
	UpdateSkillTarget();
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("SkillTarget"), SkillTargetLocation);
}

void ADeltaEnemyCharacter::EndSkillAnim(UAnimMontage* AnimMontage, bool bInterrupted)
{
	
}
