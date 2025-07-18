// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillSpitFire.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Skills/Projectiles/ProjectileBase.h"

USkillSpitFire::USkillSpitFire()
{
	SkillType = EDeltaSkillType::SpitFire;
}

void USkillSpitFire::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!InCombatComponent || !InCombatComponent->GetOwner() || !FireProjectileClass) return;

	OwnerDeltaCharacter = Cast<ADeltaBaseCharacter>(InCombatComponent->GetOwner());
	if (!OwnerDeltaCharacter.IsValid() || !OwnerDeltaCharacter->GetMesh()) return;
	if (!OwnerDeltaCharacter->GetMesh()->DoesSocketExist(SpawnSocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find %s. check Character Mesh"), *SpawnSocketName.ToString());
		return;
	}
	
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(OwnerDeltaCharacter->GetMesh()->GetSocketLocation(SpawnSocketName));
	SpawnTransform.SetRotation(OwnerDeltaCharacter->GetActorRotation().Quaternion());

	SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(FireProjectileClass, SpawnTransform, OwnerDeltaCharacter.Get());
	if (!SpawnedProjectile.Get()) return;
	
	SpawnedProjectile->SetProjectileDamage(SkillDamage);
	SpawnedProjectile->SetCombatComponent(InCombatComponent);

	SpawnedProjectile->FinishSpawning(SpawnTransform);

}
