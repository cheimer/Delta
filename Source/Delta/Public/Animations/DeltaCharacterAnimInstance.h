// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "DeltaCharacterAnimInstance.generated.h"

class ADeltaBaseCharacter;

/**
 * 
 */
UCLASS()
class DELTA_API UDeltaCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void SetBoolValue(const FString& PropertyName, const bool BoolValue);
	void SetAnimMontage(UAnimMontage* AnimMontage);
	void PlayHitReactAnim(const EDeltaHitDirection HitDirection = EDeltaHitDirection::Anywhere);

protected:
	UFUNCTION()
	void HandleMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	void HandleHitReactAnim(const float DeltaTime);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	ADeltaBaseCharacter* DeltaCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float CurrentAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsDeath = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit React", meta = (AllowPrivateAccess = "true"))
	float MaxHitReactTime = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit React", meta = (AllowPrivateAccess = "true"))
	float MinHitReactTime = 0.5f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Hit React", meta = (AllowPrivateAccess = "true"))
	float HitReactWeight = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Hit React", meta = (AllowPrivateAccess = "true"))
	float CurrentHitReactTime = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Hit React", meta = (AllowPrivateAccess = "true"))
	EDeltaHitDirection HitReactDirection;

	TWeakObjectPtr<UAnimMontage> CachedAnimMontage;

	bool bDoPlayMontage = false;
	
	bool bDoPlayHitReactAnim = false;

};
