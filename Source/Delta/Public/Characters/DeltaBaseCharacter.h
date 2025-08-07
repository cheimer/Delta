// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "GameFramework/Character.h"
#include "DeltaBaseCharacter.generated.h"

class UDeltaCharacterAnimInstance;
class UBoxComponent;
class UManaComponent;
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

	virtual void PlaySkillAnimation(const EDeltaSkillType SkillType);
	virtual void EndSkillAnimation();

	void EnableInterrupt();

	virtual void UpdateMotionWarpingTarget();
	virtual void UpdateSkillTarget();
	UBoxComponent* FindSkillCollision(const FName& SkillCollision);

	TOptional<float> GetCurrentSkillRange() const;
	
	FOnCharacterDeath OnCharacterDeath;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeSkillDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	virtual void HandleCharacterDeath(AActor* DeathActor);

	USkillDataAsset* FindSkillDataAsset(const EDeltaSkillType CurrentSkillType);

	UPROPERTY(EditAnywhere, Category = "Values")
	FString DisplayName;

	UPROPERTY()
	UDeltaCharacterAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TArray<USkillDataAsset*> SkillDataAssets;
	TWeakObjectPtr<USkillDataAsset> CachedSkillData;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UManaComponent* ManaComponent;

#pragma endregion Components
	
public:
#pragma region GetSet
	FString GetDisplayName() const {return DisplayName;}
	
	ADeltaBaseCharacter* GetCurrentSkillTarget() const {return CurrentSkillTarget.IsValid() ? CurrentSkillTarget.Get() : nullptr;}
	void SetCurrentSkillTarget(ADeltaBaseCharacter* InSkillTarget) {CurrentSkillTarget = InSkillTarget;}
	
	// return Saved TargetLocation or front location
	FVector GetSkillTargetLocation() const {return SkillTargetLocation;}
	void SetSkillTargetLocation(const FVector& InSkillTargetLocation) {SkillTargetLocation = InSkillTargetLocation;}
	
	bool GetIsDead() const {return HealthComponent->GetIsDead();}

	const TArray<TEnumAsByte<EObjectTypeQuery>>& GetTargetTraceChannel() const {return TargetTraceChannel;}
	
#pragma endregion GetSet

};
