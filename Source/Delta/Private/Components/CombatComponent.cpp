// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	if (USkillBase* Skill = NewObject<USkillBase>(this, SkillClass))
	{
		CachedSkill = Skill;
		CachedSkill->BeginSkill(this);
		CurrentSkillCount++;
	}
}

void UCombatComponent::EndSkill()
{
	if (IsValid(CachedSkill))
	{
		CachedSkill->EndSkill();
	}
}

void UCombatComponent::ReleaseSkill()
{
	CachedSkill = nullptr;
	CurrentSkillCount = 0;
}

void UCombatComponent::CancelSkill(float BlendOutTime)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter || !OwnerDeltaCharacter->GetMesh() || !OwnerDeltaCharacter->GetMesh()->GetAnimInstance()) return;

	EndSkill();
	ReleaseSkill();

	UAnimInstance* OwnerAnimIns = OwnerDeltaCharacter->GetMesh()->GetAnimInstance();
	if (OwnerAnimIns->IsAnyMontagePlaying())
	{
		OwnerAnimIns->Montage_Stop(BlendOutTime);
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

void UCombatComponent::ActInput()
{
	if (IsValid(CachedSkill))
	{
		CachedSkill->ActInput();
	}
}

void UCombatComponent::LookAtCameraCenter()
{
	ADeltaPlayableCharacter* PlayerCharacter = Cast<ADeltaPlayableCharacter>(GetOwner());
	if (!PlayerCharacter) return;

	PlayerCharacter->LookAtCameraCenter();
}

void UCombatComponent::LookAtForward()
{
	ADeltaPlayableCharacter* PlayerCharacter = Cast<ADeltaPlayableCharacter>(GetOwner());
	if (!PlayerCharacter) return;

	PlayerCharacter->LookAtForward();
}

TOptional<bool> UCombatComponent::IsFirstSection()
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return TOptional<bool>();

	return OwnerDeltaCharacter->IsFirstSection();
}

void UCombatComponent::MoveCharacterMesh(const FVector& NewLocation)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;

	OwnerDeltaCharacter->MoveCharacterMesh(NewLocation, OwnerDeltaCharacter->GetMontageRemainTime().Get(0.0f) - 0.1f);
}

void UCombatComponent::ApplyKnockback(AActor* TargetActor, const FVector& ImpulseForce)
{
	ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
	if (!TargetCharacter || !TargetCharacter->GetCharacterMovement()) return;
	
	TargetCharacter->GetCharacterMovement()->AddImpulse(ImpulseForce, true);
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
