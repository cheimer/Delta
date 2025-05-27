// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "GameFramework/Character.h"
#include "DeltaBaseCharacter.generated.h"

class USkillDataAsset;
class USkillBase;
class UHealthComponent;
class UCombatComponent;
class UMotionWarpingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeath, AActor*, DeathActor);

UCLASS()
class DELTA_API ADeltaBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADeltaBaseCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void ActiveSkill(const EDeltaSkillType SkillType);
	virtual void DeActiveSkill();

	virtual void UpdateSkillTarget();
	
	FOnCharacterDeath OnCharacterDeath;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeSkillDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	virtual void OnCharacterDeathHandle(AActor* DeathActor);

	UFUNCTION()
	void EndDeathAnim();

	USkillDataAsset* FindSkillDataAsset(const EDeltaSkillType CurrentSkillType);

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TArray<USkillDataAsset*> SkillDataAssets;

	TWeakObjectPtr<ADeltaBaseCharacter> CurrentSkillTarget = nullptr;
	FVector SkillTargetLocation;
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceChannel;
	
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

#pragma endregion Components
	
public:
#pragma region GetSet
	TWeakObjectPtr<ADeltaBaseCharacter> GetCurrentSkillTarget() const {return CurrentSkillTarget;}
	void SetCurrentSkillTarget(TWeakObjectPtr<ADeltaBaseCharacter> const InSkillTarget) {CurrentSkillTarget = InSkillTarget;}
	
	// return Saved TargetLocation or front location
	FVector GetSkillTargetLocation() const {return SkillTargetLocation;}
	void SetSkillTargetLocation(const FVector& InSkillTargetLocation) {SkillTargetLocation = InSkillTargetLocation;}
	
	TOptional<float> GetSkillDamage(EDeltaSkillType SkillType);

	bool GetIsDead() const {return HealthComponent->GetIsDead();}

	const TArray<TEnumAsByte<EObjectTypeQuery>>& GetTargetTraceChannel() const {return TargetTraceChannel;}
	
#pragma endregion GetSet

};
