// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/SaveGameInterface.h"
#include "DeltaPlayerController.generated.h"

class APostProcessHolder;
class UPostProcessComponent;
class UDamageVignetteComponent;
class ADeltaPlayableCharacter;
class ADeltaBaseCharacter;
class UInputDataAsset;
class ADeltaHUD;
/**
 *
 */

struct FBeforeCharacterData
{
	int SkillSetIndex = 0;
	int SkillKeyIndex = 0;
	float TargetArmLength = 350.0f;
};

UCLASS()
class DELTA_API ADeltaPlayerController : public APlayerController, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	ADeltaPlayerController();

	void PlayerDamaged();
	
	void TargetDetected(const AActor* Target);
	void TargetLost();

	void RegisterTeamMember(ADeltaBaseCharacter* TeamMember);
	void UnregisterTeamMember(ADeltaBaseCharacter* TeamMember);
	
	void SwitchToNextCharacter();
	void SwitchToBeforeCharacter();

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
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void HandleChangeSkillSet(int BeforeIndex, int AfterIndex);

	UFUNCTION()
	void HandleSelectSkill(int SelectSet, int SelectIndex, bool bIsSelect);

private:
	void SwitchCharacter(int32 NewIndex);

	TWeakObjectPtr<ADeltaHUD> DeltaHUD;
	TWeakObjectPtr<ADeltaPlayableCharacter> OwningPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputDataAsset* ControllerInputDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSoftObjectPtr<UWorld> MainLevel;

	bool bIsInputEnable = true;

	FDateTime LastSavedTime;

	UPROPERTY()
	TArray<ADeltaBaseCharacter*> TeamMembers;

	int32 CurrentCharacterIndex = 0;
	FBeforeCharacterData BeforeCharacterData;
	
	UPROPERTY(EditAnywhere, Category = "Switch Character")
	float FadeDuration = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "PostProcesser")
	TSubclassOf<APostProcessHolder> PostProcessHolderClass;
	UPROPERTY()
	APostProcessHolder* PostProcessHolder;

public:
#pragma region GetSet
	void SetInputEnable(const bool InInputEnable) {bIsInputEnable = InInputEnable;}
	bool GetInputEnable() const {return bIsInputEnable;}

	const TArray<ADeltaBaseCharacter*>& GetTeamMembers() const { return TeamMembers; }
	int32 GetCurrentCharacterIndex() const { return CurrentCharacterIndex; }
	ADeltaBaseCharacter* GetCurrentCharacter() const;

#pragma endregion GetSet

};
