// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillLeftClaws.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Kismet/GameplayStatics.h"

void USkillLeftClaws::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!InCombatComponent || !InCombatComponent->GetOwner()) return;

	OwnerDeltaCharacter = Cast<ADeltaBaseCharacter>(InCombatComponent->GetOwner());
	if (!OwnerDeltaCharacter) return;

	FVector AttackLocation = OwnerDeltaCharacter->GetMesh()->GetSocketLocation(SocketName);
	FVector TargetLocation = InCombatComponent->GetSkillTargetLocation(true);

	if (FVector::Distance(AttackLocation, TargetLocation) < AttackRange)
	{
		AttackTarget();
	}
}

void USkillLeftClaws::AttackTarget()
{
	TWeakObjectPtr TargetCharacter = CombatComponent->GetSkillTargetActor();
	if (!TargetCharacter.IsValid()) return;

	UE_LOG(LogTemp, Warning, TEXT("Skill left claws"));
	CombatComponent->ApplySkillDamage(TargetCharacter.Get(), CombatComponent->GetOwner(), SkillType);
}
