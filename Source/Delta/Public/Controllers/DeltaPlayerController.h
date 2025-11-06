// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/SaveGameInterface.h"
#include "DeltaPlayerController.generated.h"

class ADeltaPlayableCharacter;
class ADeltaBaseCharacter;
class UInputDataAsset;
class ADeltaHUD;
/**
 *
 */
UCLASS()
class DELTA_API ADeltaPlayerController : public APlayerController, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	void TargetDetected(const AActor* Target);
	void TargetLost();

	// Team management and character switching
	void RegisterTeamMember(ADeltaBaseCharacter* TeamMember);
	void UnregisterTeamMember(ADeltaBaseCharacter* TeamMember);
	void SwitchToNextCharacter();
	void SwitchToPreviousCharacter();
	void SwitchToCharacterByIndex(int32 Index);

	TArray<UTexture2D*>& GetSkillTextures(int Index);
	TArray<int32> GetSkillCosts(int Index);

	void SetHudVisible(const bool bIsVisible);

	void GameEnd();

	UFUNCTION()
	void Exit();

	void ContinueGame();
	void GoMain(const bool bIsSave = true);
	void QuitGame(const bool bIsSave = true);

	float GetPlayingTime();
	float GetPlayerHealth();
	float GetTotalDamage();
	bool IsPlayerWin();

#pragma region ISaveGameInterface
	virtual void SaveData_Implementation(UDeltaSaveGame* DeltaSaveGame) override;
	virtual void LoadData_Implementation(UDeltaSaveGame* DeltaSaveGame) override;

#pragma endregion ISaveGameInterface
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void HandleChangeSkillSet(int BeforeIndex, int AfterIndex);

	UFUNCTION()
	void HandleSelectSkill(int SelectSet, int SelectIndex, bool bIsSelect);

private:
	TWeakObjectPtr<ADeltaHUD> DeltaHUD;
	TWeakObjectPtr<ADeltaPlayableCharacter> OwningPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputDataAsset* ControllerInputDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSoftObjectPtr<UWorld> MainLevel;

	bool bIsInputEnable = true;

	FDateTime LastSavedTime;

	// Team management
	UPROPERTY()
	TArray<ADeltaBaseCharacter*> TeamMembers;

	int32 CurrentCharacterIndex = 0;

	void SwitchCharacter(int32 NewIndex);

public:
#pragma region GetSet
	void SetInputEnable(const bool InInputEnable) {bIsInputEnable = InInputEnable;}
	bool GetInputEnable() const {return bIsInputEnable;}

	const TArray<ADeltaBaseCharacter*>& GetTeamMembers() const { return TeamMembers; }
	int32 GetCurrentCharacterIndex() const { return CurrentCharacterIndex; }
	ADeltaBaseCharacter* GetCurrentCharacter() const;

#pragma endregion GetSet

};
