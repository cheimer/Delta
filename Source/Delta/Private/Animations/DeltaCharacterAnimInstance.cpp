// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/DeltaCharacterAnimInstance.h"

#include "Characters/DeltaBaseCharacter.h"


void UDeltaCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	DeltaCharacter = Cast<ADeltaBaseCharacter>(TryGetPawnOwner());

	bIsFlying = false;
}

void UDeltaCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	DeltaCharacter = DeltaCharacter == nullptr ? Cast<ADeltaBaseCharacter>(TryGetPawnOwner()) : DeltaCharacter;
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

		// 방향 판별 (왼쪽 / 오른쪽)
		float Direction = FVector::CrossProduct(ForwardVector, Velocity).Z >= 0 ? 1.f : -1.f;
		CurrentAngle = AngleDegrees * Direction;
	}
	else
	{
		CurrentAngle = 0.0f;
	}

	

}