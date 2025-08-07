// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/DesertDragonCharacter.h"

#include "MotionWarpingComponent.h"
#include "Animations/DeltaCharacterAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/PhaseComponent.h"
#include "DataAssets/Skill/SkillDataAsset.h"
#include "DeltaTypes/DeltaNamespaceTypes.h"
#include "GameFramework/CharacterMovementComponent.h"

ADesertDragonCharacter::ADesertDragonCharacter()
{
	GetCharacterMovement()->MaxAcceleration = 20480.0f;
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 360.0f, 0);

	PhaseComponent = CreateDefaultSubobject<UPhaseComponent>("PhaseComponent");
	

}

void ADesertDragonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PhaseComponent)
	{
		PhaseComponent->OnPhaseChanged.AddDynamic(this, &ThisClass::HandleNextPhase);
	}

}

void ADesertDragonCharacter::HandleCharacterDeath(AActor* DeathCharacter)
{
	Super::HandleCharacterDeath(DeathCharacter);

	check(GetCharacterMovement());
	check(GetCapsuleComponent());
	
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->MovementMode = MOVE_Falling;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LandedDelegate.AddDynamic(this, &ThisClass::HandleCharacterLanded);

}

void ADesertDragonCharacter::HandleNextPhase(const int PhaseNum)
{
	switch (PhaseNum)
	{
	case 1:
		DoPhaseOne();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unconfirmed Phase"));
		break;
	}
}

void ADesertDragonCharacter::DoPhaseOne()
{
	GetCharacterMovement()->MovementMode = MOVE_Flying;
	
	if (GetMesh() && FlyStartMontage)
	{
		AnimInstance = AnimInstance ? AnimInstance : Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->SetBoolValue(AnimValue_DesertDragon::IsFlying, true);
			AnimInstance->SetAnimMontage(FlyStartMontage);
			AnimInstance->RootMotionMode = ERootMotionMode::RootMotionFromEverything;

			AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnFlyEnd);

			if (MotionWarpingComponent)
			{
				FVector Destination = GetActorLocation();
				Destination += GetActorForwardVector() * FlyLocation.X;
				Destination.Z += FlyLocation.Z;
				MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName(WarpTarget::FlyLocation), Destination);
			}
		}
	}
}

void ADesertDragonCharacter::OnFlyEnd(UAnimMontage* Montage, bool bInterrupted)
{
	
	if (!GetMesh()) return;
	AnimInstance = AnimInstance ? AnimInstance : Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return;

	AnimInstance->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
	AnimInstance->OnMontageEnded.RemoveDynamic(this, &ThisClass::OnFlyEnd);
	
}

void ADesertDragonCharacter::HandleCharacterLanded(const FHitResult& Hit)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (GetMesh())
	{
		AnimInstance = AnimInstance ? AnimInstance : Cast<UDeltaCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->SetBoolValue(AnimValue_DesertDragon::IsHitGround, true);
		}
	}

}

/*
 * 0 ~ 5 : Ground Skill, 6 ~ 7 : Fly Skill
 */
void ADesertDragonCharacter::SetCurrentSkill()
{
	Super::SetCurrentSkill();
	
	if (SkillDataAssets.Num() != DRAGON_SKILL_NUM)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill num is %d, It should be %d"), SkillDataAssets.Num(), DRAGON_SKILL_NUM);
		return;
	}

	int RandIndex = FMath::RandRange(0, SkillDataAssets.Num() - 1);

	if (PhaseComponent)
	{
		switch (PhaseComponent->GetCurrentPhaseNum())
		{
		case 0:
			RandIndex = FMath::RandRange(0, 5);
			break;
		case 1:
			RandIndex = FMath::RandRange(6, 7);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Undefined Phase"));
		}
	}

	CachedSkillData = SkillDataAssets[RandIndex];
}

bool ADesertDragonCharacter::IsFlying() const
{
	if (PhaseComponent)
	{
		return PhaseComponent->GetCurrentPhaseNum() >= 1;
	}

	return false;
}
