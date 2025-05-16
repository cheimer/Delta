// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "GameFramework/Character.h"
#include "DeltaBaseCharacter.generated.h"

class USkillDataAsset;
class USkillBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);

class UHealthComponent;
class UCombatComponent;
class UMotionWarpingComponent;

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
	virtual void CharacterDeath();

	UFUNCTION()
	void EndDeathAnim();

	USkillDataAsset* FindSkillDataAsset(const EDeltaSkillType CurrentSkillType);

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TArray<USkillDataAsset*> SkillDataAssets;

	TWeakObjectPtr<USkillBase> CachedSkill;
	
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
	// return Saved TargetLocation or front location
	FVector GetSkillTargetLocation() const {return SkillTargetLocation;}
	
	virtual TOptional<float> GetSkillDamage(EDeltaSkillType SkillType) {return TOptional<float>();}

	bool GetIsDead() const {return HealthComponent->GetIsDead();}

	void SetCachedSkill(TWeakObjectPtr<USkillBase> const InSkill) {CachedSkill = InSkill;}
	
#pragma endregion GetSet

};
