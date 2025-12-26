// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Projectiles/ProjectileBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CombatComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Subsystem/ObjectPoolSubsystem.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>("ProjectileCollision");
	SetRootComponent(ProjectileCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->bAutoActivate = false;
	
	InitialLifeSpan = 0.0f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileCollision)
	{
		ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileOverlap);
		ProjectileCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
		
		ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (ProjectileMovement)
	{
		DefaultVelocity = ProjectileMovement->Velocity;
		
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->SetComponentTickEnabled(false);
	}
	
}

void AProjectileBase::SpawnProjectile(const UObject* WorldContext, UClass* SpawnClass, AActor* SpawnerActor, UCombatComponent* InCombatComponent,
	const FTransform& SpawnTransform, const float SkillDamage)
{
	if (!WorldContext || !SpawnClass || !SpawnerActor || !InCombatComponent) return;
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return;
	
	UObjectPoolSubsystem* ObjectPool = UObjectPoolSubsystem::Get(World);
	if (!ObjectPool) return;
	
	AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(ObjectPool->GetObject(SpawnClass));
	if (!SpawnedProjectile) return;

	SpawnedProjectile->SetBeginSkill(SpawnerActor, InCombatComponent, SpawnTransform, SkillDamage);
}

void AProjectileBase::SetBeginSkill(AActor* SpawnerActor, UCombatComponent* InCombatComponent, const FTransform& SpawnTransform, const float SkillDamage)
{
	if (!SpawnerActor || !InCombatComponent) return;
	
	SetOwner(SpawnerActor);
	CombatComponent = InCombatComponent;
	SetActorTransform(SpawnTransform);
	ProjectileDamage = SkillDamage;

	ExcludeActors.Empty();
	ExcludeActors.Add(SpawnerActor);

	if (ProjectileCollision)
	{
		ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->SetComponentTickEnabled(true);
		ProjectileMovement->Velocity = DefaultVelocity;
		SetDirection(GetActorForwardVector().GetSafeNormal());
		ProjectileMovement->Activate(true);
	}

	GetWorld()->GetTimerManager().SetTimer(ReturnPoolTimer, this, &ThisClass::ReturnPool, ProjectileDuration, false);
}

void AProjectileBase::ReturnPool()
{
	GetWorld()->GetTimerManager().ClearTimer(ReturnPoolTimer);

	if (ProjectileMovement)
	{
		ProjectileMovement->Deactivate();
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->SetComponentTickEnabled(false);
	}

	if (ProjectileCollision)
	{
		ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	ExcludeActors.Empty();
	
	CombatComponent.Reset();
	SetOwner(nullptr);

	if (UObjectPoolSubsystem* ObjectPool = UObjectPoolSubsystem::Get(GetWorld()))
	{
		ObjectPool->ReturnObject(this);
	}
	else
	{
		Destroy();
	}
}

void AProjectileBase::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetOwner() || !CombatComponent.IsValid()) return;
	
	if (ExcludeActors.Contains(OtherActor)) return;
	ExcludeActors.Add(OtherActor);

	if (CombatComponent->GetIsOpponent(OtherActor).IsSet() && CombatComponent->GetIsOpponent(OtherActor).GetValue())
	{
		CombatComponent->ApplySkillDamage(OtherActor, GetOwner(), ProjectileDamage);

		if (ProjectileHitVFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileHitVFX,
				GetActorLocation(), GetActorRotation(), GetActorScale(),
				true, true, ENCPoolMethod::AutoRelease, true);
		}
		
		ReturnPool();
	}
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ProjectileHitVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileHitVFX,
			GetActorLocation(), GetActorRotation(), GetActorScale(),
			true, true, ENCPoolMethod::AutoRelease, true);
	}
	
	ReturnPool();
}

void AProjectileBase::SetDirection(const FVector& Direction) const
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->Velocity.Size();
}

void AProjectileBase::SetGravityScale(const float GravityScale)
{
	ProjectileMovement->ProjectileGravityScale = GravityScale;
}
