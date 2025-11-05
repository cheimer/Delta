// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillBlowAway.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"

USkillBlowAway::USkillBlowAway()
{
	SkillType = EDeltaSkillType::BlowAway;
}

void USkillBlowAway::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!CombatComponent.IsValid() && !CombatComponent->GetOwner()) return;
	AActor* OwnerActor = CombatComponent->GetOwner();
	
	TArray<AActor*> HitActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	CombatComponent->GetTargetTraceChannel(ObjectTypes);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), OwnerActor->GetActorLocation(), Range,
		ObjectTypes, ADeltaBaseCharacter::StaticClass(),TArray<AActor*>(), HitActors);
	
	OverlappedActors.Add(OwnerActor);
	
	for (auto HitActor : HitActors)
	{
		if (!HitActor) continue;

		if (CombatComponent->GetIsOpponent(HitActor).IsSet() && CombatComponent->GetIsOpponent(HitActor).GetValue())
		{
			if (!OverlappedActors.Contains(HitActor))
			{
				OverlappedActors.Add(HitActor);
				
				FVector TargetLocation = HitActor->GetActorLocation();
				FVector Direction = (TargetLocation - OwnerActor->GetActorLocation()).GetSafeNormal();
				Direction.Z = 0.0f;
			
				CombatComponent->ApplyKnockback(HitActor, Direction * KnockbackPower);
			}
		}
	}
	
}
