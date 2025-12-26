// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCollisionAttack.h"
#include "Skills/SkillBase.h"
#include "SkillSpreadFire.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class DELTA_API USkillSpreadFire : public USkillCollisionAttack
{
	GENERATED_BODY()

public:
	USkillSpreadFire();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;
	virtual void EndSkill() override;

protected:
	virtual void OnTargetOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnTargetEscape(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnAttackOverlapped();

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Values")
	float TickRate = 0.2f;
	
};
