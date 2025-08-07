// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Projectiles/ProjectileBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CombatComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>("ProjectileCollision");
	SetRootComponent(ProjectileCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is not set owner"), *GetName());
		return;
	}
	ExcludeActors.Add(GetOwner());
	
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileOverlap);
	ProjectileCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	SetLifeSpan(ProjectileDuration);
}

void AProjectileBase::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetOwner() || !CombatComponent.IsValid()) return;
	
	if (ExcludeActors.Contains(OtherActor)) return;
	ExcludeActors.Add(OtherActor);

	if (CombatComponent->GetIsOpponent(OtherActor))
	{
		CombatComponent->ApplySkillDamage(OtherActor, GetOwner(), ProjectileDamage);

		if (ProjectileHitVFX->IsValid())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileHitVFX, GetActorLocation());
		}
	}
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ProjectileHitVFX->IsValid())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileHitVFX, GetActorLocation());
	}
	Destroy();
}

void AProjectileBase::SetDirection(const FVector& Direction) const
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->Velocity.Size();
}
