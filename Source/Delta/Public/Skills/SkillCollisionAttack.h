// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillCollisionAttack.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class DELTA_API USkillCollisionAttack : public USkillBase
{
	GENERATED_BODY()

public:
	USkillCollisionAttack();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;
	virtual void EndSkill() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Skill")
	float SkillDamage = 50;
	
	UFUNCTION()
	virtual void OnTargetOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, Category = "Name", meta = (AllowPrivateAccess = "true"))
	FName CollisionName = "";

	TWeakObjectPtr<UBoxComponent> SkillCollisionBox;
	TSet<TWeakObjectPtr<AActor>> OverlappedActors;
	
	UPROPERTY()
	ADeltaBaseCharacter* OwnerDeltaCharacter;

};
