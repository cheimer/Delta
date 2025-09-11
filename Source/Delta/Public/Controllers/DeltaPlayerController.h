// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DeltaPlayerController.generated.h"

class ADeltaHUD;
/**
 * 
 */
UCLASS()
class DELTA_API ADeltaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void TargetDetected(const AActor* Target);
	void TargetLost();

	TArray<UTexture2D*>& GetSkillTextures(int Index);
	TArray<int32> GetSkillCosts(int Index);

	void SetHudVisible(const bool bIsVisible);


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleChangeSkillSet(int BeforeIndex, int AfterIndex);

	UFUNCTION()
	void HandleSelectSkill(int SelectSet, int SelectIndex, bool bIsSelect);

private:
	TWeakObjectPtr<ADeltaHUD> DeltaHUD;

	bool bIsInputEnable = true;

public:
#pragma region GetSet
	void SetInputEnable(const bool InInputEnable) {bIsInputEnable = InInputEnable;}
	bool GetInputEnable() const {return bIsInputEnable;}

#pragma endregion GetSet
	
};
