// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillCollisionAttack.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CombatComponent.h"

USkillCollisionAttack::USkillCollisionAttack()
{

}

void USkillCollisionAttack::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!InCombatComponent || !InCombatComponent->GetOwner()) return;

	OwnerDeltaCharacter = Cast<ADeltaBaseCharacter>(InCombatComponent->GetOwner());
	if (!OwnerDeltaCharacter) return;

	SkillCollisionBox = OwnerDeltaCharacter->FindSkillCollision(CollisionName);
	if (!SkillCollisionBox.Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find Skill CollisionName. Check Character Blueprint."));
		return;
	}

	OverlappedActors.Add(OwnerDeltaCharacter);
	
	SkillCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTargetOverlap);
	SkillCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TArray<AActor*> OverlappingActors;
	SkillCollisionBox->GetOverlappingActors(OverlappingActors);
	
	for (auto OverlapActor : OverlappingActors)
	{
		if (!OverlappedActors.Contains(OverlapActor))
		{
			OverlappedActors.Add(OverlapActor);
			CombatComponent->ApplySkillDamage(OverlapActor, CombatComponent->GetOwner(), SkillDamage);
		}
	}

}

void USkillCollisionAttack::EndSkill()
{
	Super::EndSkill();

	if (!SkillCollisionBox.IsValid()) return;

	SkillCollisionBox->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnTargetOverlap);
	SkillCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverlappedActors.Empty();
	
}

void USkillCollisionAttack::OnTargetOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !CombatComponent.IsValid() || !CombatComponent->GetIsOpponent(OtherActor)) return;

	if (OverlappedActors.Contains(OtherActor)) return;

	OverlappedActors.Add(OtherActor);
	
	CombatComponent->ApplySkillDamage(OtherActor, CombatComponent->GetOwner(), SkillDamage);
	bIsTargetOverlapping = true;
}
