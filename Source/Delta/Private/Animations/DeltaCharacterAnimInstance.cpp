// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/DeltaCharacterAnimInstance.h"

#include "Characters/DeltaBaseCharacter.h"


void UDeltaCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	DeltaCharacter = Cast<ADeltaBaseCharacter>(TryGetPawnOwner());

}

void UDeltaCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	DeltaCharacter = DeltaCharacter ? DeltaCharacter : Cast<ADeltaBaseCharacter>(TryGetPawnOwner());
	if(!DeltaCharacter) return;

	FVector Velocity = DeltaCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	CurrentSpeed = Velocity.Size();

	if (!FMath::IsNearlyZero(Velocity.Size()))
	{
		FVector ForwardVector = DeltaCharacter->GetActorForwardVector();
		ForwardVector.Z = 0.0f;
		
		Velocity.Normalize();
		ForwardVector.Normalize();

		float AngleRadians = FMath::Acos(FVector::DotProduct(ForwardVector, Velocity));
		float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

		float Direction = FVector::CrossProduct(ForwardVector, Velocity).Z >= 0 ? 1.f : -1.f;
		CurrentAngle = AngleDegrees * Direction;

		// Interrupt Montage
		if (DeltaCharacter->GetCanInterruptSkill())
		{
			if (IsAnyMontagePlaying())
			{
				Montage_Stop(0.5f);
			}
		}
	}
	else
	{
		CurrentAngle = 0.0f;
	}

	if (CachedAnimMontage.IsValid() && bDoPlayMontage)
	{
		Montage_Play(CachedAnimMontage.Get());
		bDoPlayMontage = false;
		
		if (!OnMontageEnded.IsAlreadyBound(this, &ThisClass::HandleMontageEnd))
		{
			OnMontageEnded.AddDynamic(this, &ThisClass::HandleMontageEnd);
		}
	}

	HandleHitReactAnim(DeltaTime);

}

void UDeltaCharacterAnimInstance::SetBoolValue(const FString& PropertyName, const bool BoolValue)
{
	FBoolProperty* Property = FindFProperty<FBoolProperty>(GetClass(), *PropertyName);
	if(Property)
	{
		Property->SetPropertyValue_InContainer(this, BoolValue);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find property from %s"), *GetName());
	}
}

void UDeltaCharacterAnimInstance::SetAnimMontage(UAnimMontage* AnimMontage)
{
	if (AnimMontage)
	{
		CachedAnimMontage = AnimMontage;
		bDoPlayMontage = true;
	}
}

void UDeltaCharacterAnimInstance::HandleMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	OnMontageEnded.RemoveDynamic(this, &ThisClass::HandleMontageEnd);
	
	DeltaCharacter = DeltaCharacter ? DeltaCharacter : Cast<ADeltaBaseCharacter>(TryGetPawnOwner());
	check(DeltaCharacter);

	DeltaCharacter->EndSkillAnimation();

}

void UDeltaCharacterAnimInstance::PlayHitReactAnim(const EDeltaHitDirection HitDirection)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit : %s"), *(StaticEnum<EDeltaHitDirection>()->GetNameStringByIndex(static_cast<int32>(HitDirection))));
	if (bDoPlayHitReactAnim && CurrentHitReactTime < MinHitReactTime) return;
	
	bDoPlayHitReactAnim = true;
	
	HitReactDirection = HitDirection;
	HitReactWeight = 1.0f;
	CurrentHitReactTime = 0.0f;
}

void UDeltaCharacterAnimInstance::HandleHitReactAnim(const float DeltaTime)
{
	if (!bDoPlayHitReactAnim) return;
	
	CurrentHitReactTime += DeltaTime;
	if (CurrentHitReactTime > MaxHitReactTime)
	{
		HitReactWeight = 0.0f;
		CurrentHitReactTime = 0.0f;
		bDoPlayHitReactAnim = false;
	}
}
