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
	if (Skill.IsValid())
	{
		CachedSkill = Skill;
		CachedSkill->BeginSkill(this);
	}
}

void UCombatComponent::EndSkill()
{
	if (CachedSkill.IsValid())
	{
		CachedSkill->EndSkill();
		CachedSkill = nullptr;
	}
}

void UCombatComponent::ApplySkillDamage(AActor* DamagedActor, AActor* DamageCauser, EDeltaSkillType SkillType)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;
	
	TOptional<float> DefaultSkillDamage = OwnerDeltaCharacter->GetSkillDamage(SkillType);
	if (!DefaultSkillDamage.IsSet()) return;

	UGameplayStatics::ApplyDamage(DamagedActor, DefaultSkillDamage.GetValue() * DamageMultiplier,
		OwnerDeltaCharacter->GetInstigatorController(), DamageCauser, UDamageType::StaticClass());
}

void UCombatComponent::TakeDamage()
{
	if (CachedSkill.IsValid())
	{
		CachedSkill->ReactDamaged();
	}
}

TOptional<bool> UCombatComponent::GetIsOpponent(const AActor* CheckActor)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return TOptional<bool>();

	if (OwnerDeltaCharacter->GetClass() != CheckActor->GetClass())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UCombatComponent::GetTargetTraceChannel(TArray<TEnumAsByte<EObjectTypeQuery>>& OutObjectTypes)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;

	OutObjectTypes = OwnerDeltaCharacter->GetTargetTraceChannel();
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
