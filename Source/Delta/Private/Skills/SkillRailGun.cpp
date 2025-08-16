// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillRailGun.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Kismet/GameplayStatics.h"


USkillRailGun::USkillRailGun()
{
	SkillType = EDeltaSkillType::RailGun;
}

void USkillRailGun::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!InCombatComponent) return;

	ADeltaBaseCharacter* OwnerDeltaCharacter = Cast<ADeltaBaseCharacter>(InCombatComponent->GetOwner());
	if (!IsValid(OwnerDeltaCharacter) || !OwnerDeltaCharacter->GetMesh()) return;
	if (!OwnerDeltaCharacter->GetMesh()->DoesSocketExist(SpawnSocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find %s. check Character Mesh"), *SpawnSocketName.ToString());
		return;
	}
	if (!RailGunVFX)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find VFX"));
		return;
	}
	
	FVector SpawnLocation = OwnerDeltaCharacter->GetMesh()->GetSocketLocation(SpawnSocketName);
	FRotator SpawnRotation = (OwnerDeltaCharacter->GetSkillTargetLocation() - SpawnLocation).Rotation();
	
	SpawnVFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, RailGunVFX, SpawnLocation, SpawnRotation);

	FVector EndLocation = SpawnLocation + SpawnRotation.Vector().GetSafeNormal() * VFXSpeed * SpawnDuration;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	CombatComponent->GetTargetTraceChannel(ObjectTypes);
	TArray<FHitResult> HitResults;
	
	UKismetSystemLibrary::LineTraceMultiForObjects(this, SpawnLocation, EndLocation, ObjectTypes,
		false, TArray<AActor*>(), EDrawDebugTrace::None, HitResults, true);

	TArray<AActor*> DamagedActor;
	for (auto Hit : HitResults)
	{
		if (ADeltaBaseCharacter* HitActor = Cast<ADeltaBaseCharacter>(Hit.GetActor()))
		{
			if (InCombatComponent->GetIsOpponent(HitActor) && !DamagedActor.Contains(HitActor))
			{
				InCombatComponent->ApplySkillDamage(HitActor, InCombatComponent->GetOwner(), SkillDamage);
				DamagedActor.Add(HitActor);
			}
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(VFXTimer, this, &ThisClass::MoveVFX, 0.1f, true);

}

void USkillRailGun::MoveVFX()
{
	if (SpawnVFX)
	{
		SpawnVFX->AddRelativeLocation(SpawnVFX->GetForwardVector() * VFXSpeed);
	}

	CurrentSpawnedTime += 0.1f;
	if (CurrentSpawnedTime > SpawnDuration)
	{
		if (GetWorld()->GetTimerManager().TimerExists(VFXTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(VFXTimer);
		}

		if (SpawnVFX)
		{
			SpawnVFX->DestroyInstance();			
		}
	}
}
