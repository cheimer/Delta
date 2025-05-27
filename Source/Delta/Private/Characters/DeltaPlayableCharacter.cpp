// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DeltaPlayableCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MotionWarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/DeltaEnemyCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "DataAssets/Input/InputDataAsset.h"
#include "DataAssets/Skill/SkillDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ADeltaPlayableCharacter::ADeltaPlayableCharacter()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 200.0f;
	TargetArmLengthGoTo = 200.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 55.0f, 65.0f);
	SpringArmComponent->ProbeSize = 1.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraRotationLagSpeed = 25.0f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
}

void ADeltaPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(PlayerInputDataAsset);
	
	if(HasAuthority())
	{
		if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(PlayerInputDataAsset->InputMappingContext, 0);
			}
		}
	}
}

void ADeltaPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(PlayerInputDataAsset);
	check(PlayerInputDataAsset->InputMappingContext);
	
	if(!HasAuthority())
	{
		if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(PlayerInputDataAsset->InputMappingContext, 0);
			}
		}
	}

}

void ADeltaPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!FMath::IsNearlyEqual(SpringArmComponent->TargetArmLength, TargetArmLengthGoTo))
	{
		SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength, TargetArmLengthGoTo, DeltaTime * 10.0f);
	}
	
	if (CurrentStatus == EPlayerStatus::LockTarget || (CurrentStatus == EPlayerStatus::Skill && CachedStatus == EPlayerStatus::LockTarget))
	{
		if (CurrentLockTarget.IsValid() && !CurrentLockTarget->GetIsDead())
		{
			FRotator TargetRot = (CurrentLockTarget->GetActorLocation() - GetActorLocation()).Rotation();
			SpringArmComponent->SetWorldRotation(TargetRot);
		}
		else
		{
			SetLockTarget(false);
		}
	}

	if (bIsWaitingSkill)
	{
		WaitingSkillTime += DeltaTime;
		if (WaitingSkillTime > 0.1f)
		{
			WaitingSkillTime = 0.0f;
			UpdateSkillTarget();
			if (CurrentSkillTarget.IsValid())
			{
				DrawDebugSphere(GetWorld(), CurrentSkillTarget->GetActorLocation(), 300.0f, 32, FColor::Red);
			}
		}
	}
}

void ADeltaPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	check(EnhancedInput && PlayerInputDataAsset);
	
	for (const auto& [InputActionPtr, TriggerEvent, FunctionName] : PlayerInputDataAsset->InputActionConfigs)
	{
		if (!FindFunction(FunctionName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't find function in %s : %s"), *FunctionName.ToString(), *GetName());
			continue;
		}
		EnhancedInput->BindAction(InputActionPtr, TriggerEvent, this, FunctionName);
	}
	
}

void ADeltaPlayableCharacter::StartWaitingSkill(FName SkillIndex)
{
	if (CurrentStatus == EPlayerStatus::Skill)
		return;

	if (HealthComponent->GetIsDead()) return;
	
	if (!SkillIndexToSkillType.Find(SkillIndex)) return;
	
	CachedSkillData = FindSkillDataAsset(SkillIndexToSkillType[SkillIndex]);
	bIsWaitingSkill = true;
	WaitingSkillTime = 1.0f;

	if (CachedSkillData->Type == EDeltaSkillType::Parrying)
	{
		BeginSkillAnim();
	}
	
}

void ADeltaPlayableCharacter::CancelWaitingSkill()
{
	if (CurrentStatus == EPlayerStatus::Skill)
		return;

	bIsWaitingSkill = false;

}

void ADeltaPlayableCharacter::BeginSkillAnim()
{
	if (!bIsWaitingSkill || CurrentStatus == EPlayerStatus::Skill)
		return;

	if (HealthComponent->GetIsDead()) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	if (!CachedSkillData.IsValid()) return;
	
	CachedStatus = CurrentStatus;
	CurrentStatus = EPlayerStatus::Skill;

	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &ADeltaPlayableCharacter::EndSkillAnim);

	AnimInstance->Montage_Play(CachedSkillData->AnimMontage);
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, CachedSkillData->AnimMontage);
	
	UpdateSkillTarget();
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("SkillTarget"), SkillTargetLocation);
	
	bIsWaitingSkill = false;
}

void ADeltaPlayableCharacter::EndSkillAnim(UAnimMontage* AnimMontage, bool bInterrupted)
{
	CurrentStatus = CachedStatus;
}

void ADeltaPlayableCharacter::SetLockTarget(bool bWantsLockOn)
{
	if (bWantsLockOn)
	{
		CurrentStatus = EPlayerStatus::LockTarget;
		
		SpringArmComponent->bUsePawnControlRotation = false;
		
		UpdateLockTarget();
	}
	else
	{
		CurrentStatus = EPlayerStatus::Default;
		
		Controller->SetControlRotation(CameraComponent->GetComponentRotation());
		
		SpringArmComponent->bUsePawnControlRotation = true;
		
		CurrentLockTarget = nullptr;
	}
	
}

void ADeltaPlayableCharacter::SetLockTargetWhileSkill(bool bWantsLockOn)
{
	if (bWantsLockOn)
	{
		CachedStatus = EPlayerStatus::LockTarget;
		
		SpringArmComponent->bUsePawnControlRotation = false;
		
		UpdateLockTarget();
	}
	else
	{
		CachedStatus = EPlayerStatus::Default;
		
		Controller->SetControlRotation(CameraComponent->GetComponentRotation());
		
		SpringArmComponent->bUsePawnControlRotation = true;
		
		CurrentLockTarget = nullptr;
	}
}

void ADeltaPlayableCharacter::UpdateLockTarget()
{
	CurrentLockTarget = FindEnemyFromFront();
}

void ADeltaPlayableCharacter::UpdateSkillTarget()
{
	CurrentSkillTarget = FindEnemyFromFront();

	// Case1: Hit Enemy
	if (CurrentSkillTarget.IsValid())
	{
		SkillTargetLocation = CurrentSkillTarget->GetActorLocation();
	}
	// Case2: Hit Notting, TargetLocation set forward to 1000.0f
	else
	{
		FVector StartLocation = CameraComponent->GetComponentLocation();
		FVector NoTargetLocation = StartLocation + CameraComponent->GetForwardVector() * 1000.0f;
		NoTargetLocation.Z = GetActorLocation().Z;
	
		SkillTargetLocation = NoTargetLocation;
	}
}

ADeltaBaseCharacter* ADeltaPlayableCharacter::FindEnemyFromFront() const
{
	TArray<FHitResult> HitResults;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + CameraComponent->GetForwardVector() * 5000.0f;

	UKismetSystemLibrary::BoxTraceMultiForObjects(this, StartLocation, EndLocation, FVector(1.0f, 100.0f, 100.0f),
		GetActorRotation(), TargetTraceChannel, false,TArray<AActor*>(), EDrawDebugTrace::None, HitResults, true);

	for (auto HitResult : HitResults)
	{
		if (Cast<ADeltaEnemyCharacter>(HitResult.GetActor()))
		{
			return Cast<ADeltaBaseCharacter>(HitResult.GetActor());
		}
	}

	return nullptr;
}

void ADeltaPlayableCharacter::OnCharacterDeathHandle(AActor* DeathActor)
{
	Super::OnCharacterDeathHandle(DeathActor);

	
}

#pragma region Input

void ADeltaPlayableCharacter::Move(const FInputActionValue& Value)
{
	if (CurrentStatus == EPlayerStatus::Skill)
		return;

	if (HealthComponent->GetIsDead()) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	FRotator MovementRotation;
	if (CurrentStatus == EPlayerStatus::Default)
	{
		MovementRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	}
	else if (CurrentStatus == EPlayerStatus::LockTarget)
	{
		MovementRotation = FRotator(0.0f, CameraComponent->GetComponentRotation().Yaw, 0.0f);
	}
	
	if (MovementVector.Y != 0.0f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (MovementVector.X != 0.0f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADeltaPlayableCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (CurrentStatus != EPlayerStatus::LockTarget)
	{
		if (LookAxisVector.X != 0.0f)
		{
			AddControllerYawInput(LookAxisVector.X);
		}
		if (LookAxisVector.Y != 0.0f)
		{
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void ADeltaPlayableCharacter::Scroll(const FInputActionValue& Value)
{
	const float ScrollValue = Value.Get<float>();

	if (ScrollValue > 0.0f)
	{
		TargetArmLengthGoTo = FMath::Clamp(TargetArmLengthGoTo - ScrollSensitive, TargetArmLengthMin, TargetArmLengthMax);
	}
	else if (ScrollValue < 0.0f)
	{
		TargetArmLengthGoTo = FMath::Clamp(TargetArmLengthGoTo + ScrollSensitive, TargetArmLengthMin, TargetArmLengthMax);
	}
	
}

void ADeltaPlayableCharacter::LockTarget(const FInputActionValue& Value)
{
	if (HealthComponent->GetIsDead()) return;

	if (CurrentStatus == EPlayerStatus::Default)
	{
		SetLockTarget(true);
	}
	else if (CurrentStatus == EPlayerStatus::LockTarget)
	{
		SetLockTarget(false);
	}
	else if (CurrentStatus == EPlayerStatus::Skill)
	{
		if (CachedStatus == EPlayerStatus::Default)
		{
			SetLockTargetWhileSkill(true);
		}
		else if (CachedStatus == EPlayerStatus::LockTarget)
		{
			SetLockTargetWhileSkill(false);
		}
	}
}

void ADeltaPlayableCharacter::Execute(const FInputActionValue& Value)
{
	if (bIsWaitingSkill)
	{
		BeginSkillAnim();
	}
}

void ADeltaPlayableCharacter::Parrying(const FInputActionValue& Value)
{
	if (bIsWaitingSkill)
	{
		CancelWaitingSkill();
	}
	else
	{
		StartWaitingSkill("Parrying");
	}
}

void ADeltaPlayableCharacter::SkillFirst(const FInputActionValue& Value)
{
	StartWaitingSkill("SkillFirst");
}

void ADeltaPlayableCharacter::SkillSecond(const FInputActionValue& Value)
{
	StartWaitingSkill("SkillSecond");
}

void ADeltaPlayableCharacter::SkillThird(const FInputActionValue& Value)
{
	StartWaitingSkill("SkillThird");
}

void ADeltaPlayableCharacter::SkillBefore(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Before"));
}

void ADeltaPlayableCharacter::SkillNext(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Next"));
}

#pragma endregion Input
