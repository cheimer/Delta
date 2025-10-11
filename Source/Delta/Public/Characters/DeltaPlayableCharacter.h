// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Characters/DeltaBaseCharacter.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "DeltaPlayableCharacter.generated.h"

class ADeltaPlayerController;
class USkillDataAsset;
class UInputDataAsset;
class UCameraComponent;
class USpringArmComponent;
class USkillBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeSkillSet, int, BeforeIndex, int, AfterIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSelectSkill, int, SelectSet, int, SelectIndex, bool, bIsSelect);

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	Default,
	Skill,
	LockTarget,
	WaitingSkill
};

USTRUCT(BlueprintType)
struct FSkillTypeWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EDeltaSkillType> SkillTypes;

	FSkillTypeWrapper()
	{
		// UI Skill Size
		SkillTypes.SetNum(3);
	}

};


/**
 * 
 */
UCLASS()
class DELTA_API ADeltaPlayableCharacter : public ADeltaBaseCharacter
{
	GENERATED_BODY()

public:
	ADeltaPlayableCharacter();

	virtual void UpdateSkillTarget() override;

	virtual void PlaySkillAnimation(const EDeltaSkillType SkillType) override;
	virtual void EndSkillAnimation() override;

	TArray<UTexture2D*>& GetSkillTextures(const int Index);
	TArray<int32> GetSkillCosts(const int Index);

	void LookAtCameraCenter();
	void LookAtForward();

	FOnChangeSkillSet OnChangeSkillSet;
	FOnSelectSkill OnSelectSkill;

#pragma region ISaveGameInterface
	virtual void SaveData_Implementation(UDeltaSaveGame* DeltaSaveGame) override;
	virtual void LoadData_Implementation(UDeltaSaveGame* DeltaSaveGame) override;

#pragma endregion ISaveGameInterface
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void HandleCharacterDeath(AActor* DeathActor) override;
	
	TWeakObjectPtr<ADeltaBaseCharacter> CurrentLockTarget = nullptr;
	
#pragma region Inputs
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void Scroll(const FInputActionValue& Value);
	
	UFUNCTION()
	void LockTarget(const FInputActionValue& Value);
	
	UFUNCTION()
	void Execute(const FInputActionValue& Value);
	
	UFUNCTION()
	void Parrying(const FInputActionValue& Value);
	
	UFUNCTION()
	void SkillFirst(const FInputActionValue& Value);
	
	UFUNCTION()
	void SkillSecond(const FInputActionValue& Value);
	
	UFUNCTION()
	void SkillThird(const FInputActionValue& Value);
	
	UFUNCTION()
	void SkillBefore(const FInputActionValue& Value);
	
	UFUNCTION()
	void SkillNext(const FInputActionValue& Value);
	
#pragma endregion Inputs

private:
	void StartWaitingSkill(int KeyIndex);
	void StartWaitingSkill(EDeltaSkillType SkillType);
	void CancelWaitingSkill();

	void ChangeSkillList(const bool bIsNext);

	void SetLockTarget(bool bWantsLockOn);
	
	void UpdateLockTarget();
	ADeltaBaseCharacter* FindEnemyFromFront() const;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TArray<FSkillTypeWrapper> SkillSetArray;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputDataAsset* PlayerInputDataAsset;
	
	EPlayerStatus CurrentStatus = EPlayerStatus::Default;
	EPlayerStatus CachedStatus = EPlayerStatus::Default;
	
	float WaitingSkillTime = 0.0f;

	int CurrentSkillSetIndex = 0;
	int CurrentSkillKeyIndex = 0;

	double CachedPitch = 0.0f;
	bool bIsLookingCameraCenter = false;
	bool bIsTargetUpdate = false;

	TWeakObjectPtr<ADeltaPlayerController> PlayerController;
	
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;
	
#pragma endregion Components

#pragma region Setting Values
	const float TargetArmLengthMin = 100.0f;
	const float TargetArmLengthMax = 500.0f;
	const float ScrollSensitive = 20.0f;
	float TargetArmLengthGoTo = 350.0f;

#pragma endregion Setting Values
	
public:
#pragma region GetSet
	int GetCurrentSkillSetIndex() const { return CurrentSkillSetIndex; };
	
#pragma endregion GetSet

};
