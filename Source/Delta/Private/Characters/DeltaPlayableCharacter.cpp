// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DeltaPlayableCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MotionWarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Enemy/DeltaEnemyCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Controllers/DeltaPlayerController.h"
#include "DataAssets/Input/InputDataAsset.h"
#include "DataAssets/Skill/SkillDataAsset.h"
#include "DeltaTypes/DeltaNamespaceTypes.h"
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
	GetCharacterMovement()->RotationRate = FRotator(500.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = TargetArmLengthGoTo;
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

	bool bIsLockTarget =
		CurrentStatus == EPlayerStatus::LockTarget ||
		((CurrentStatus == EPlayerStatus::WaitingSkill || CurrentStatus == EPlayerStatus::Skill) && CachedStatus == EPlayerStatus::LockTarget);
	
	if (bIsLockTarget)
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
	
	if (CurrentStatus == EPlayerStatus::WaitingSkill)
	{
		WaitingSkillTime += DeltaTime;
		if (WaitingSkillTime > 0.1f)
		{
			WaitingSkillTime = 0.0f;
			UpdateSkillTarget();
			if (CurrentSkillTarget.IsValid())
			{
				// TODO: Need to add UI.
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

void ADeltaPlayableCharacter::StartWaitingSkill(int KeyIndex)
{
	if (CurrentStatus != EPlayerStatus::Default && CurrentStatus != EPlayerStatus::LockTarget) return;

	if (HealthComponent->GetIsDead()) return;

	if (!SkillSetArray.IsValidIndex(CurrentSkillSetIndex)) return;
	if (!SkillSetArray[CurrentSkillSetIndex].SkillTypes.IsValidIndex(KeyIndex)) return;
	
	CachedSkillData = FindSkillDataAsset(SkillSetArray[CurrentSkillSetIndex].SkillTypes[KeyIndex]);
	if (!CachedSkillData.IsValid()) return;
	
	CachedStatus = CurrentStatus;
	CurrentStatus = EPlayerStatus::WaitingSkill;
	WaitingSkillTime = 1.0f;

}

void ADeltaPlayableCharacter::StartWaitingSkill(EDeltaSkillType SkillType)
{
	CachedSkillData = FindSkillDataAsset(SkillType);
	if (!CachedSkillData.IsValid()) return;
	
	CachedStatus = CurrentStatus;
	CurrentStatus = EPlayerStatus::WaitingSkill;
	WaitingSkillTime = 1.0f;

	if (CachedSkillData->Type == EDeltaSkillType::Parrying)
	{
		PlaySkillAnimation(SkillType);
	}
	
}

void ADeltaPlayableCharacter::CancelWaitingSkill()
{
	if (CurrentStatus != EPlayerStatus::WaitingSkill)
		return;

	CurrentStatus = CachedStatus;

}

void ADeltaPlayableCharacter::ChangeSkillList(const bool bIsNext)
{
	if (CurrentStatus != EPlayerStatus::Default && CurrentStatus != EPlayerStatus::LockTarget) return;

	if (CurrentSkillSetIndex == 0 && !bIsNext) return;
	if (CurrentSkillSetIndex == SkillSetArray.Num() - 1 && bIsNext) return;

	int BeforeSkillSetIndex = CurrentSkillSetIndex;
	CurrentSkillSetIndex = FMath::Clamp(BeforeSkillSetIndex + (bIsNext ? 1 : -1), 0, SkillSetArray.Num() - 1);

	OnChangeSkillSet.Broadcast(BeforeSkillSetIndex, CurrentSkillSetIndex);
}

void ADeltaPlayableCharacter::PlaySkillAnimation(const EDeltaSkillType SkillType)
{
	if (CurrentStatus != EPlayerStatus::WaitingSkill) return;
	if (HealthComponent->GetIsDead()) return;
	if (!CachedSkillData.IsValid()) return;
	
	Super::PlaySkillAnimation(SkillType);
	
	CurrentStatus = EPlayerStatus::Skill;

	UpdateSkillTarget();
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName(WarpTarget::SkillTarget), SkillTargetLocation);
}

void ADeltaPlayableCharacter::EndSkillAnimation()
{
	Super::EndSkillAnimation();
	
	CurrentStatus = CachedStatus;
}

TArray<UTexture2D*>& ADeltaPlayableCharacter::GetSkillTextures(const int Index)
{
	static TArray<UTexture2D*> TextureArray;
	TextureArray.Empty();
	
	if (!SkillSetArray.IsValidIndex(Index)) return TextureArray;
	
	TArray<EDeltaSkillType> SkillTypes;
	SkillTypes.Append(SkillSetArray[Index].SkillTypes);

	for (auto SkillType : SkillTypes)
	{
		if (USkillDataAsset* SkillData = FindSkillDataAsset(SkillType))
		{
			TextureArray.Add(SkillData->MainImage);
		}
	}

	return TextureArray;
}

void ADeltaPlayableCharacter::LookAtCameraCenter()
{
	if (!SpringArmComponent) return;

	SetLockTarget(false);
	bIsLookingCameraCenter = true;

	CachedPitch = GetActorRotation().Pitch;

	SpringArmComponent->SetRelativeRotation(FRotator::ZeroRotator);
	
	SpringArmComponent->bUsePawnControlRotation = false;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

}

void ADeltaPlayableCharacter::LookAtForward()
{
	if (!SpringArmComponent) return;

	SpringArmComponent->bUsePawnControlRotation = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	FRotator ResetRotation = FRotator(CachedPitch, GetActorRotation().Yaw, GetActorRotation().Roll);
	SetActorRotation(ResetRotation);

	bIsLookingCameraCenter = false;
}

void ADeltaPlayableCharacter::SetLockTarget(bool bWantsLockOn)
{
	if (bWantsLockOn)
	{
		if (CurrentStatus == EPlayerStatus::Skill || CurrentStatus == EPlayerStatus::WaitingSkill)
		{
			CachedStatus = EPlayerStatus::LockTarget;
		}
		else
		{
			CurrentStatus = EPlayerStatus::LockTarget;
		}
		
		SpringArmComponent->bUsePawnControlRotation = false;
		
		UpdateLockTarget();

		ADeltaPlayerController* PlayerController = Cast<ADeltaPlayerController>(Controller);
		if (CurrentLockTarget.IsValid() && PlayerController)
		{
			PlayerController->LockTargetDetected(CurrentLockTarget.Get());
		}
	}
	else
	{
		if (CurrentStatus == EPlayerStatus::Skill || CurrentStatus == EPlayerStatus::WaitingSkill)
		{
			CachedStatus = EPlayerStatus::Default;
		}
		else
		{
			CurrentStatus = EPlayerStatus::Default;
		}
		
		Controller->SetControlRotation(CameraComponent->GetComponentRotation());
		
		SpringArmComponent->bUsePawnControlRotation = true;
		
		CurrentLockTarget = nullptr;

		if (ADeltaPlayerController* PlayerController = Cast<ADeltaPlayerController>(Controller))
		{
			PlayerController->LockTargetLost();
		}
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

void ADeltaPlayableCharacter::HandleCharacterDeath(AActor* DeathActor)
{
	Super::HandleCharacterDeath(DeathActor);

	
}

#pragma region Input

void ADeltaPlayableCharacter::Move(const FInputActionValue& Value)
{
	if (HealthComponent->GetIsDead()) return;

	if (CurrentStatus == EPlayerStatus::Skill && !bCanInterruptSkill) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();
	FRotator MovementRotation;

	EPlayerStatus CheckStatus = (CurrentStatus == EPlayerStatus::Skill || CurrentStatus == EPlayerStatus::WaitingSkill) ? CachedStatus : CurrentStatus;
	if (CheckStatus == EPlayerStatus::Default)
	{
		MovementRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	}
	else if (CheckStatus == EPlayerStatus::LockTarget)
	{
		MovementRotation = FRotator(0.0f, CameraComponent->GetComponentRotation().Yaw, 0.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Undefined status. Check Move Func"));
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

	bool bCanLookUp = CurrentStatus == EPlayerStatus::Default ||
		((CurrentStatus == EPlayerStatus::WaitingSkill || CurrentStatus == EPlayerStatus::Skill) && CachedStatus == EPlayerStatus::Default);

	if (bCanLookUp)
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
	if (HealthComponent->GetIsDead() || bIsLookingCameraCenter) return;

	switch (CurrentStatus)
	{
	case EPlayerStatus::Default:
		SetLockTarget(true);
		break;
	case EPlayerStatus::LockTarget:
		SetLockTarget(false);
		break;
	case EPlayerStatus::WaitingSkill:
	case EPlayerStatus::Skill:
		if (CachedStatus == EPlayerStatus::Default)
			SetLockTarget(true);
		else if (CachedStatus == EPlayerStatus::LockTarget)
			SetLockTarget(false);
		else
			UE_LOG(LogTemp, Warning, TEXT("Undefined status. Lock Target"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Undefined status. Lock Target"));
		break;
	}
}

void ADeltaPlayableCharacter::Execute(const FInputActionValue& Value)
{
	switch (CurrentStatus)
	{
	case EPlayerStatus::Default:
	case EPlayerStatus::LockTarget:
		return;
	case EPlayerStatus::WaitingSkill:
		if (CachedSkillData.IsValid())
		{
			PlaySkillAnimation(CachedSkillData->Type);
		}
		return;
	case EPlayerStatus::Skill:
		if (IsValid(CombatComponent))
		{
			CombatComponent->ActInput();
		}
		return;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Undefined status. Execute Func"));
	}
	
}

void ADeltaPlayableCharacter::Parrying(const FInputActionValue& Value)
{
	switch (CurrentStatus)
	{
	case EPlayerStatus::Default:
	case EPlayerStatus::LockTarget:
		StartWaitingSkill(EDeltaSkillType::Parrying);
	case EPlayerStatus::Skill:
		return;
	case EPlayerStatus::WaitingSkill:
		CancelWaitingSkill();
		return;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Undefined status. Parrying Func"));
	}
}

void ADeltaPlayableCharacter::SkillFirst(const FInputActionValue& Value)
{
	StartWaitingSkill(0);
}

void ADeltaPlayableCharacter::SkillSecond(const FInputActionValue& Value)
{
	StartWaitingSkill(1);
}

void ADeltaPlayableCharacter::SkillThird(const FInputActionValue& Value)
{
	StartWaitingSkill(2);
}

void ADeltaPlayableCharacter::SkillBefore(const FInputActionValue& Value)
{
	ChangeSkillList(false);
}

void ADeltaPlayableCharacter::SkillNext(const FInputActionValue& Value)
{
	ChangeSkillList(true);
}

#pragma endregion Input
