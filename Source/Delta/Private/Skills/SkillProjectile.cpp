// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillProjectile.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Skills/Projectiles/ProjectileBase.h"

void USkillProjectile::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!InCombatComponent || !InCombatComponent->GetOwner() || !ProjectileClass) return;

	ADeltaBaseCharacter* OwnerDeltaCharacter = Cast<ADeltaBaseCharacter>(InCombatComponent->GetOwner());
	if (!IsValid(OwnerDeltaCharacter) || !OwnerDeltaCharacter->GetMesh()) return;
	if (!OwnerDeltaCharacter->GetMesh()->DoesSocketExist(SpawnSocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find %s. check Character Mesh"), *SpawnSocketName.ToString());
		return;
	}
	
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(OwnerDeltaCharacter->GetMesh()->GetSocketLocation(SpawnSocketName));
	SpawnTransform.SetRotation(OwnerDeltaCharacter->GetActorRotation().Quaternion());

	SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileClass, SpawnTransform, OwnerDeltaCharacter);
	if (!SpawnedProjectile.Get()) return;
	
	SpawnedProjectile->SetProjectileDamage(SkillDamage);
	SpawnedProjectile->SetCombatComponent(InCombatComponent);

	SpawnedProjectile->FinishSpawning(SpawnTransform);

}
