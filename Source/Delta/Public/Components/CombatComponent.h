// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "CombatComponent.generated.h"


class USkillBase;
class ADeltaBaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	void BeginSkill(TSubclassOf<USkillBase>& SkillClass);
	void EndSkill();

	void ApplySkillDamage(AActor* DamagedActor, AActor* DamageCauser, EDeltaSkillType SkillType);

	void TakeDamage();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ADeltaBaseCharacter* OwnerDeltaCharacter;
	
	TWeakObjectPtr<USkillBase> CachedSkill;

	float DamageMultiplier = 1.0f;
	float DamageTakenMultiplier = 1.0f;

#pragma region GetSet
public:
	TOptional<bool> GetIsOpponent(const AActor* CheckActor);
	
	void GetTargetTraceChannel(TArray<TEnumAsByte<EObjectTypeQuery>>& OutObjectTypes);

	TWeakObjectPtr<ADeltaBaseCharacter> GetSkillTargetActor();
	FVector GetSkillTargetLocation(bool bIsUpdateSkillTargetLocation);

	void SetDamageMultiplier(const float InDamageMultiplier) {DamageMultiplier = InDamageMultiplier;}
	float GetDamageMultiplier() const {return DamageMultiplier;}
	void SetDamageTakenMultiplier(const float InDamageTakenMultiplier) {DamageTakenMultiplier = InDamageTakenMultiplier;}
	float GetDamageTakenMultiplier() const {return DamageTakenMultiplier;}

#pragma endregion  GetSet

};
