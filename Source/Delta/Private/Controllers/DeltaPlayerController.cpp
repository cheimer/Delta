// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DeltaPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "Components/SlateWrapperTypes.h"
#include "DataAssets/Input/InputDataAsset.h"
#include "GameModes/DeltaBaseGameMode.h"
#include "Helper/DeltaDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/DeltaSaveGame.h"
#include "Subsystem/SaveGameSubsystem.h"
#include "UI/DeltaHUD.h"

void ADeltaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(ControllerInputDataAsset && ControllerInputDataAsset->InputMappingContext);

	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputSubsystem->AddMappingContext(ControllerInputDataAsset->InputMappingContext, static_cast<int32>(EInputPriority::Controller));
		
	}

	OwningPlayerCharacter = OwningPlayerCharacter.IsValid()? OwningPlayerCharacter : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (OwningPlayerCharacter.IsValid())
	{
		OwningPlayerCharacter->OnChangeSkillSet.AddDynamic(this, &ThisClass::HandleChangeSkillSet);
		OwningPlayerCharacter->OnSelectSkill.AddDynamic(this, &ThisClass::HandleSelectSkill);
	}

	DeltaHUD = Cast<ADeltaHUD>(GetHUD());
}

void ADeltaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent && ControllerInputDataAsset);

	for (const auto& [InputActionPtr, TriggerEvent, FunctionName] : ControllerInputDataAsset->InputActionConfigs)
	{
		if (!FindFunction(FunctionName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't find function in %s : %s"), *FunctionName.ToString(), *GetName());
			continue;
		}
		
		EnhancedInputComponent->BindAction(InputActionPtr, TriggerEvent, this, FunctionName);
	}
}

void ADeltaPlayerController::HandleChangeSkillSet(int BeforeIndex, int AfterIndex)
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->ChangeSkillSet(BeforeIndex, AfterIndex);
	
}

void ADeltaPlayerController::HandleSelectSkill(int SelectSet, int SelectIndex, bool bIsSelect)
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->SelectSkill(SelectSet, SelectIndex, bIsSelect);
	
}

void ADeltaPlayerController::TargetDetected(const AActor* Target)
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->ShowTarget(Target);
	
}

void ADeltaPlayerController::TargetLost()
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->HideTarget();
	
}

TArray<UTexture2D*>& ADeltaPlayerController::GetSkillTextures(int Index)
{
	OwningPlayerCharacter = OwningPlayerCharacter.IsValid()? OwningPlayerCharacter : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (OwningPlayerCharacter.IsValid())
	{
		return OwningPlayerCharacter->GetSkillTextures(Index);
	}
	else
	{
		static TArray<UTexture2D*> EmptyArray;
		return EmptyArray;
	}
	
}

TArray<int32> ADeltaPlayerController::GetSkillCosts(int Index)
{
	OwningPlayerCharacter = OwningPlayerCharacter.IsValid()? OwningPlayerCharacter : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (OwningPlayerCharacter.IsValid())
	{
		return OwningPlayerCharacter->GetSkillCosts(Index);
	}
	else
	{
		return TArray<int32>();
	}
}

void ADeltaPlayerController::SetHudVisible(const bool bIsVisible)
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;
	
	DeltaHUD->SetHudVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
}

void ADeltaPlayerController::GameEnd()
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;

	SetInputEnable(false);
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
	
	DeltaHUD->ChangeWidgetType(EDeltaWidgetType::Result);
}

void ADeltaPlayerController::Exit()
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;

	SetInputEnable(false);
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	DeltaHUD->ChangeWidgetType(EDeltaWidgetType::Pause);
}

void ADeltaPlayerController::ContinueGame()
{
	DeltaHUD = DeltaHUD.IsValid() ? DeltaHUD.Get() : Cast<ADeltaHUD>(GetHUD());
	if (!DeltaHUD.IsValid()) return;

	SetInputEnable(true);
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
	
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	DeltaHUD->ChangeWidgetType(EDeltaWidgetType::Play);
}

void ADeltaPlayerController::GoMain()
{
	if (USaveGameSubsystem* SaveGameSubsystem = USaveGameSubsystem::Get(GetWorld()))
	{
		SaveGameSubsystem->SaveGame();
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainLevel);
	
}

void ADeltaPlayerController::QuitGame()
{
	if (USaveGameSubsystem* SaveGameSubsystem = USaveGameSubsystem::Get(GetWorld()))
	{
		SaveGameSubsystem->SaveGame();
	}
	
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}

float ADeltaPlayerController::GetPlayingTime()
{
	ADeltaBaseGameMode* GameMode = Cast<ADeltaBaseGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return 0.0f;

	return GameMode->GetPlayingTime();
}

float ADeltaPlayerController::GetPlayerHealth()
{
	OwningPlayerCharacter = OwningPlayerCharacter.IsValid()? OwningPlayerCharacter : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (!OwningPlayerCharacter.IsValid()) return 0.0f;

	UHealthComponent* OwnerHealthComp = OwningPlayerCharacter->GetComponentByClass<UHealthComponent>();
	if (!OwnerHealthComp) return 0.0f;

	return OwnerHealthComp->GetCurrentHealth();
}

float ADeltaPlayerController::GetTotalDamage()
{
	OwningPlayerCharacter = OwningPlayerCharacter.IsValid()? OwningPlayerCharacter : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (!OwningPlayerCharacter.IsValid()) return 0.0f;

	return OwningPlayerCharacter->GetTotalDealing();
}

bool ADeltaPlayerController::IsPlayerWin()
{
	ADeltaBaseGameMode* GameMode = Cast<ADeltaBaseGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return false;

	return GameMode->IsPlayerWin();
}

#pragma region ISaveGameInterface
void ADeltaPlayerController::SaveData_Implementation(UDeltaSaveGame* DeltaSaveGame)
{
	if (!DeltaSaveGame || !GetWorld()) return;

	ADeltaBaseGameMode* GameMode = Cast<ADeltaBaseGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	DeltaSaveGame->PlayingTime = GameMode->GetPlayingTime();
	DeltaSaveGame->SavedTime = FDateTime::Now();
}

void ADeltaPlayerController::LoadData_Implementation(UDeltaSaveGame* DeltaSaveGame)
{
	if (!DeltaSaveGame || !GetWorld()) return;

	ADeltaBaseGameMode* GameMode = Cast<ADeltaBaseGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	GameMode->SetPlayingTime(DeltaSaveGame->PlayingTime);
	LastSavedTime = DeltaSaveGame->SavedTime;
}

#pragma endregion ISaveGameInterface
