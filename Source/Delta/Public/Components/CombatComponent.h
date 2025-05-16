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
	void EndSkill(USkillBase* CachedSkill);

	void ApplySkillDamage(AActor* DamagedActor, AActor* DamageCauser, EDeltaSkillType SkillType);

	TWeakObjectPtr<ADeltaBaseCharacter> GetSkillTargetActor();
	FVector GetSkillTargetLocation(bool bIsUpdateSkillTargetLocation);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ADeltaBaseCharacter* OwnerDeltaCharacter;

};
