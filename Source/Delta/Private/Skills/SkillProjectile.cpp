// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillProjectile.h"

#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Skills/Projectiles/ProjectileBase.h"

void USkillProjectile::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!InCombatComponent || !InCombatComponent->GetOwner() || !ProjectileClass) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(InCombatComponent->GetOwner());
	if (!IsValid(OwnerCharacter) || !OwnerCharacter->GetMesh()) return;
	if (!OwnerCharacter->GetMesh()->DoesSocketExist(SpawnSocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find %s. check Character Mesh"), *SpawnSocketName.ToString());
		return;
	}
	
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(OwnerCharacter->GetMesh()->GetSocketLocation(SpawnSocketName));
	SpawnTransform.SetRotation(OwnerCharacter->GetActorRotation().Quaternion());

	SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileClass, SpawnTransform, OwnerCharacter);
	if (!SpawnedProjectile.Get()) return;
	
	SpawnedProjectile->SetProjectileDamage(SkillDamage);
	SpawnedProjectile->SetCombatComponent(InCombatComponent);

	SpawnedProjectile->FinishSpawning(SpawnTransform);

}
