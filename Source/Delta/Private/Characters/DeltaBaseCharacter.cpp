// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DeltaBaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ADeltaBaseCharacter::ADeltaBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
	TargetTraceChannel.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TargetTraceChannel.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
}

void ADeltaBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADeltaBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::TakeSkillDamage);
	OnCharacterDeath.AddDynamic(this, &ThisClass::CharacterDeath);
}

void ADeltaBaseCharacter::TakeSkillDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->TakeDamage(Damage);
}

void ADeltaBaseCharacter::CharacterDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	
	AnimInstance->Montage_Play(DeathMontage);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::EndDeathAnim, DeathMontage->GetPlayLength());
				
}

void ADeltaBaseCharacter::EndDeathAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("DeathAnim End"));
}

void ADeltaBaseCharacter::ActiveSkill(const EDeltaSkillType SkillType)
{
}

void ADeltaBaseCharacter::DeActiveSkill()
{
}

void ADeltaBaseCharacter::UpdateSkillTarget()
{
	
}
