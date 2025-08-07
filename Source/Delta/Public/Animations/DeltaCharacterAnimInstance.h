// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

	void EnableInterrupt();
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	ADeltaBaseCharacter* DeltaCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float CurrentAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsDeath = false;

	TWeakObjectPtr<UAnimMontage> CachedAnimMontage;

	bool bDoPlayMontage = false;

	bool bCanStopMontage = false;

};
