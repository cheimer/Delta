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
	void LockTargetDetected(AActor* Target) const;
	void LockTargetLost() const;

	TArray<const UTexture2D*>& GetSkillTextures(int Index);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleChangeSkillSet(int BeforeIndex, int AfterIndex);
	
};
