// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/DeltaEnemyCharacter.h"
#include "Interfaces/Flying.h"
#include "DesertDragonCharacter.generated.h"

#define DRAGON_SKILL_NUM 8

class UPhaseComponent;
class UFloatingPawnMovement;
/**
 * 
 */
UCLASS()
class DELTA_API ADesertDragonCharacter : public ADeltaEnemyCharacter, public IFlying
{
	GENERATED_BODY()
	
public:
	ADesertDragonCharacter();

	virtual void BeginPlay() override;

#pragma region IFlying
	virtual bool IsFlying() const override;

#pragma endregion IFlying

protected:
	virtual void OnCharacterDeathHandle(AActor* DeathCharacter) override;
	
	UFUNCTION()
	void OnNextPhase(const int PhaseNum);
	
	virtual void SetCurrentSkill() override;

	UFUNCTION()
	void OnFlyEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "FlyStart")
	UAnimMontage* FlyStartMontage;
	
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhaseComponent* PhaseComponent;

#pragma endregion Components

private:
	void DoPhaseOne();
	
	UPROPERTY(EditAnywhere, Category = "FlyStart", meta = (AllowPrivateAccess = "true"))
	FVector FlyLocation = FVector(300.0f, 0.0f, 700.0f);

};
