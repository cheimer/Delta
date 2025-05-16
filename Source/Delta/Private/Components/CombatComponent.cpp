// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Skills/SkillBase.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	OwnerDeltaCharacter = nullptr;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	
}

void UCombatComponent::BeginSkill(TSubclassOf<USkillBase>& SkillClass)
{
	if (!SkillClass) return;

	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;

	TWeakObjectPtr Skill = NewObject<USkillBase>(this, SkillClass);
	if (Skill.Get())
	{
		OwnerDeltaCharacter->SetCachedSkill(Skill);
		Skill->BeginSkill(this);
	}
}

void UCombatComponent::EndSkill(USkillBase* CachedSkill)
{
	if (CachedSkill)
	{
		CachedSkill->EndSkill();
	}
}

void UCombatComponent::ApplySkillDamage(AActor* DamagedActor, AActor* DamageCauser, EDeltaSkillType SkillType)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;
	
	TOptional<float> SkillDamage = OwnerDeltaCharacter->GetSkillDamage(SkillType);
	if (!SkillDamage.IsSet()) return;
	
	UGameplayStatics::ApplyDamage(DamagedActor, SkillDamage.GetValue(), OwnerDeltaCharacter->GetInstigatorController(), DamageCauser, UDamageType::StaticClass());
}

TWeakObjectPtr<ADeltaBaseCharacter> UCombatComponent::GetSkillTargetActor()
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	check(OwnerDeltaCharacter);

	return OwnerDeltaCharacter->GetCurrentSkillTarget();
}

FVector UCombatComponent::GetSkillTargetLocation(bool bIsUpdateSkillTargetLocation)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	check(OwnerDeltaCharacter);

	if (bIsUpdateSkillTargetLocation)
	{
		OwnerDeltaCharacter->UpdateSkillTarget();
	}
	
	return OwnerDeltaCharacter->GetSkillTargetLocation();
}
