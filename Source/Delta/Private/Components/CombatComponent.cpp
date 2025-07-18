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

void UCombatComponent::BeginSkill(const TSubclassOf<USkillBase>& SkillClass)
{
	if (!SkillClass) return;

	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;

	if (USkillBase* Skill = NewObject<USkillBase>(this, SkillClass))
	{
		CachedSkill = Skill;
		CachedSkill->BeginSkill(this);
	}
}

void UCombatComponent::EndSkill()
{
	if (IsValid(CachedSkill))
	{
		CachedSkill->EndSkill();
		CachedSkill = nullptr;
	}
}

void UCombatComponent::ApplySkillDamage(AActor* DamagedActor, AActor* DamageCauser, const float SkillDamage)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;

	UE_LOG(LogTemp, Warning, TEXT("ApplySkillDamage %s to %s"), *DamageCauser->GetName(), *DamagedActor->GetName());
	
	UGameplayStatics::ApplyDamage(DamagedActor, SkillDamage * DamageMultiplier,
		OwnerDeltaCharacter->GetInstigatorController(), DamageCauser, UDamageType::StaticClass());
}

void UCombatComponent::TakeDamage()
{
	if (IsValid(CachedSkill))
	{
		CachedSkill->ReactDamaged();
	}
}

TOptional<bool> UCombatComponent::GetIsOpponent(const AActor* CheckActor)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return TOptional<bool>();
	if (!IsValid(CheckActor)) return TOptional<bool>();

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

ADeltaBaseCharacter* UCombatComponent::GetSkillTargetActor()
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	check(OwnerDeltaCharacter);

	if (!IsValid(OwnerDeltaCharacter->GetCurrentSkillTarget())) return nullptr;
	
	return OwnerDeltaCharacter->GetCurrentSkillTarget();
}

FVector UCombatComponent::GetSkillTargetLocation(const bool bIsUpdateSkillTargetLocation)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	check(OwnerDeltaCharacter);

	if (bIsUpdateSkillTargetLocation)
	{
		OwnerDeltaCharacter->UpdateSkillTarget();
	}
	
	return OwnerDeltaCharacter->GetSkillTargetLocation();
}
