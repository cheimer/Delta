// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillShield.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"

USkillShield::USkillShield()
{
	SkillType = EDeltaSkillType::Shield;
}

void USkillShield::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!InCombatComponent || !InCombatComponent->GetOwner() || !InCombatComponent->GetOwner())
	{
		check(false);
		return;
	}
	ACharacter* OwnerCharacter = Cast<ACharacter>(InCombatComponent->GetOwner());
	if (!OwnerCharacter || !OwnerCharacter->GetMesh() || !OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		check(false);
		return;
	}
	AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &ThisClass::ShieldActivating, DurationTime, true);
	
	CombatComponent->SetDamageTakenMultiplier(0.0f);
	
	bExitLoop = true;
	
}

void USkillShield::ReactDamaged()
{
	Super::ReactDamaged();
	
	if (!CombatComponent.IsValid() || !CombatComponent->GetOwner()) return;

	if (ShieldReactVFX)
	{
		FVector SpawnLocation = CombatComponent->GetOwner()->GetActorLocation();
		FRotator SpawnRotation = CombatComponent->GetOwner()->GetActorRotation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ShieldReactVFX, SpawnLocation, SpawnRotation);
	}

	bExitLoop = false;
	
}

void USkillShield::EndSkill()
{
	Super::EndSkill();
	
	CombatComponent->SetDamageTakenMultiplier(1.0f);
	GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
}

void USkillShield::ShieldActivating()
{
	if (bExitLoop)
	{
		AnimInstance->Montage_JumpToSection(FName("LoopEnd"));
		
		GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
	}
	else
	{
		bExitLoop = true;
	}
	
}
