// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillProjectile.h"

#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Skills/Projectiles/ProjectileBase.h"
#include "Subsystem/ObjectPoolSubsystem.h"

void USkillProjectile::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!InCombatComponent || !InCombatComponent->GetOwner() || !ProjectileClass) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(InCombatComponent->GetOwner());
	if (!IsValid(OwnerCharacter) || !OwnerCharacter->GetMesh()) return;
	if (!OwnerCharacter->GetMesh()->DoesSocketExist(SpawnSocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find %s socket : %s. check Character Mesh"), *OwnerCharacter->GetName(), *SpawnSocketName.ToString());
	}
	
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(OwnerCharacter->GetMesh()->GetSocketLocation(SpawnSocketName));
	SpawnTransform.SetRotation(OwnerCharacter->GetActorRotation().Quaternion());

	AProjectileBase::SpawnProjectile(this, ProjectileClass, OwnerCharacter, InCombatComponent, SpawnTransform, SkillDamage);

}
