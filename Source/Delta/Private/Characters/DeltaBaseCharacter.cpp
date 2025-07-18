// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DeltaBaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "Animations/DeltaCharacterAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ManaComponent.h"
#include "DataAssets/Skill/SkillDataAsset.h"
#include "DeltaTypes/DeltaNamespaceTypes.h"

ADeltaBaseCharacter::ADeltaBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	ManaComponent = CreateDefaultSubobject<UManaComponent>(TEXT("ManaComponent"));
	
	TargetTraceChannel.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TargetTraceChannel.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
}

void ADeltaBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

}

void ADeltaBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::TakeSkillDamage);
	OnCharacterDeath.AddDynamic(this, &ThisClass::OnCharacterDeathHandle);
}

void ADeltaBaseCharacter::TakeSkillDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->TakeDamage(Damage);
}

void ADeltaBaseCharacter::OnCharacterDeathHandle(AActor* DeathActor)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UDeltaCharacterAnimInstance* AnimInstance = Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return;
	
	AnimInstance->SetBoolValue(AnimValue::IsDeath, true);
}

USkillDataAsset* ADeltaBaseCharacter::FindSkillDataAsset(const EDeltaSkillType CurrentSkillType)
{
	for (const auto SkillData : SkillDataAssets)
	{
		if (SkillData->Type == CurrentSkillType)
		{
			return SkillData;
		}
	}

	return nullptr;
}

void ADeltaBaseCharacter::ActiveSkill(const EDeltaSkillType SkillType)
{
	if (!CombatComponent) return;

	USkillDataAsset* SkillData = FindSkillDataAsset(SkillType);
	if (!SkillData) return;

	CombatComponent->BeginSkill(SkillData->Skill);
}

void ADeltaBaseCharacter::DeActiveSkill()
{
	if (!CombatComponent) return;
	
	CombatComponent->EndSkill();
}

void ADeltaBaseCharacter::PlaySkillAnimation(const EDeltaSkillType SkillType)
{
	USkillDataAsset* SkillData = FindSkillDataAsset(SkillType);
	if (!SkillData) return;

	UDeltaCharacterAnimInstance* AnimInstance = GetMesh() ? Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance()) : nullptr;
	if (!AnimInstance) return;

	AnimInstance->SetAnimMontage(SkillData->AnimMontage);
}

void ADeltaBaseCharacter::EndSkillAnimation()
{
	
}

void ADeltaBaseCharacter::UpdateMotionWarpingTarget()
{
	if (GetCurrentSkillTarget() && MotionWarpingComponent)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName(WarpTarget::SkillTarget), GetCurrentSkillTarget()->GetActorLocation());
	}
}

void ADeltaBaseCharacter::UpdateSkillTarget()
{
	
}

UBoxComponent* ADeltaBaseCharacter::FindSkillCollision(const FName& SkillCollision)
{
	return Cast<UBoxComponent>(GetDefaultSubobjectByName(SkillCollision));
}

TOptional<float> ADeltaBaseCharacter::GetCurrentSkillRange() const
{
	if (!CachedSkillData.IsValid()) return TOptional<float>();

	return CachedSkillData->Distance;
}
