// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DeltaBaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "Animations/DeltaCharacterAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ManaComponent.h"
#include "DataAssets/Skill/SkillDataAsset.h"
#include "DeltaTypes/DeltaNamespaceTypes.h"
#include "GameUserSettings/FrontGameUserSettings.h"
#include "Helper/DeltaDebugHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SaveGame/DeltaSaveGame.h"
#include "Subsystem/HitStopSubsystem.h"

ADeltaBaseCharacter::ADeltaBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	ManaComponent = CreateDefaultSubobject<UManaComponent>(TEXT("ManaComponent"));
	
	TargetTraceChannel.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TargetTraceChannel.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
}

void ADeltaBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ADeltaBaseCharacter::SaveData_Implementation(UDeltaSaveGame* DeltaSaveGame)
{
	
}

void ADeltaBaseCharacter::LoadData_Implementation(UDeltaSaveGame* DeltaSaveGame)
{
	
}

void ADeltaBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::TakeSkillDamage);
	OnCharacterDeath.AddDynamic(this, &ThisClass::HandleCharacterDeath);

}

void ADeltaBaseCharacter::TakeSkillDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->TakeDamage(Damage, DamageCauser);

	check(IsValid(GetMesh()));
	
	AnimInstance = AnimInstance ? AnimInstance : Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return;

	EDeltaHitDirection HitDirection = CalcDirection(DamagedActor->GetActorForwardVector(),
		DamageCauser->GetActorLocation() - DamagedActor->GetActorLocation());
	AnimInstance->PlayHitReactAnim(HitDirection);
}

void ADeltaBaseCharacter::HandleCharacterDeath(AActor* DeathActor)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (!GetMesh()) return;
	AnimInstance = AnimInstance ? AnimInstance : Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return;

	AnimInstance->StopAllMontages(0.5f);
	AnimInstance->SetBoolValue(AnimValue::IsDeath, true);
}

USkillDataAsset* ADeltaBaseCharacter::FindSkillDataAsset(const EDeltaSkillType CurrentSkillType)
{
	for (const auto SkillData : SkillDataAssets)
	{
		if (SkillData && SkillData->Type == CurrentSkillType)
		{
			return SkillData;
		}
	}

	return nullptr;
}

void ADeltaBaseCharacter::ActiveSkill(const EDeltaSkillType SkillType)
{
	if (!CombatComponent) return;

	USkillDataAsset* SkillData = FindSkillDataAsset(SkillType);
	if (!SkillData) return;

	CombatComponent->BeginSkill(SkillData->Skill);
}

void ADeltaBaseCharacter::DeActiveSkill()
{
	if (!CombatComponent) return;
	
	CombatComponent->EndSkill();
}

void ADeltaBaseCharacter::PlaySkillAnimation(const EDeltaSkillType SkillType)
{
	USkillDataAsset* SkillData = FindSkillDataAsset(SkillType);
	if (!SkillData) return;

	if (!GetMesh()) return;
	AnimInstance = AnimInstance ? AnimInstance : Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return;
	
	if (!ManaComponent) return;

	AnimInstance->SetAnimMontage(SkillData->AnimMontage);
	ManaComponent->UseSkill(SkillData->Cost);
}

void ADeltaBaseCharacter::EndSkillAnimation()
{
	if (!CombatComponent) return;
	
	CombatComponent->ReleaseSkill();
	
	bCanInterruptSkill = false;
}

float ADeltaBaseCharacter::GetSkillDurationTime(const EDeltaSkillType SkillType)
{
	USkillDataAsset* SkillData = FindSkillDataAsset(SkillType);
	if (!SkillData || !SkillData->AnimMontage) return 0.0f;

	return SkillData->AnimMontage->GetPlayLength();
}

void ADeltaBaseCharacter::EnableInterrupt()
{
	bCanInterruptSkill = true;
}

void ADeltaBaseCharacter::UpdateMotionWarpingTarget()
{
	if (!MotionWarpingComponent) return;

	if (GetCurrentSkillTarget())
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName(WarpTarget::SkillTarget), GetCurrentSkillTarget()->GetActorLocation());
	}
	else
	{
		MotionWarpingComponent->RemoveWarpTarget(FName(WarpTarget::SkillTarget));
	}
}

void ADeltaBaseCharacter::UpdateSkillTarget()
{
	
}

UBoxComponent* ADeltaBaseCharacter::FindSkillCollision(const FName& SkillCollision)
{
	return Cast<UBoxComponent>(GetDefaultSubobjectByName(SkillCollision));
}

TOptional<float> ADeltaBaseCharacter::GetCurrentSkillRange() const
{
	if (!CachedSkillData.IsValid()) return TOptional<float>();

	return CachedSkillData->Distance;
}

TOptional<bool> ADeltaBaseCharacter::IsFirstSection()
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance()) return TOptional<bool>();

	AnimInstance = AnimInstance ? AnimInstance : Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return TOptional<bool>();

	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (!CurrentMontage) return TOptional<bool>();
	
	int32 CurrentSection = CurrentMontage->GetSectionIndex(AnimInstance->Montage_GetCurrentSection());

	switch (CurrentSection)
	{
	case INDEX_NONE:
		return TOptional<bool>();
	case 0:
		return true;
	default:
		return false;
	}
}

TOptional<float> ADeltaBaseCharacter::GetMontageRemainTime()
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance()) return TOptional<float>();

	AnimInstance = AnimInstance ? AnimInstance : Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return TOptional<float>();

	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (!CurrentMontage) return TOptional<float>();
	
	return CurrentMontage->GetPlayLength() - AnimInstance->Montage_GetPosition(CurrentMontage);
}

void ADeltaBaseCharacter::MoveCharacterMesh(const FVector& NewLocation, const float DurationTime)
{
	if (!GetMesh()) return;

	CachedMeshLocation = GetMesh()->GetRelativeLocation();
	GetMesh()->SetRelativeLocation(CachedMeshLocation + NewLocation);

	GetWorld()->GetTimerManager().SetTimer(RestoreMeshTimerHandle, this, &ThisClass::RestoreCharacterMeshLocation, DurationTime, false);
}

void ADeltaBaseCharacter::SetVisibleMesh(const FName MeshName, const bool bIsVisible)
{
	UStaticMeshComponent* SMMesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(MeshName));
	if (!SMMesh) return;

	SMMesh->SetVisibility(bIsVisible);
}

bool ADeltaBaseCharacter::BeginAttackDilation(const float MaxDuration, const float TimeDilation)
{
	if (!GetWorld()) return false;

	UHitStopSubsystem* HitStopSubsystem = UHitStopSubsystem::Get(GetWorld());
	if (!HitStopSubsystem) return false;

	return HitStopSubsystem->StartHitStop(MaxDuration, TimeDilation, EHitStopPriority::Player);
}

void ADeltaBaseCharacter::EndAttackDilation()
{
	if (!GetWorld()) return;

	UHitStopSubsystem* HitStopSubsystem = UHitStopSubsystem::Get(GetWorld());
	if (!HitStopSubsystem) return;
	
	HitStopSubsystem->EndHitStop();
	
}

void ADeltaBaseCharacter::RestoreCharacterMeshLocation()
{
	if (!GetMesh()) return;

	GetMesh()->SetRelativeLocation(CachedMeshLocation);
}

EDeltaHitDirection ADeltaBaseCharacter::CalcDirection(const FVector& DamagedForward, const FVector& ToAttackerDirection)
{
	FVector ForwardNormal = DamagedForward.GetSafeNormal();
	FVector AttackerNormal = ToAttackerDirection.GetSafeNormal();

	float AngleRadians = FMath::Acos(FVector::DotProduct(ForwardNormal, AttackerNormal));
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	float Direction = FVector::CrossProduct(ForwardNormal, AttackerNormal).Z >= 0 ? 1.f : -1.f;
	float CurrentAngle = AngleDegrees * Direction;

	if (CurrentAngle > -45.0f && CurrentAngle < 45.0f) return EDeltaHitDirection::Forward;
	if (CurrentAngle > 45.0f && CurrentAngle < 135.0f) return EDeltaHitDirection::Right;
	if (CurrentAngle < -45.0f && CurrentAngle > -135.0f) return EDeltaHitDirection::Left;

	return EDeltaHitDirection::Backward;

}
