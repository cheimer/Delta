// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DeltaAIController.h"
#include "DeltaAllyController.generated.h"

class ADeltaPlayerController;
class ADeltaBaseCharacter;

/**
 *
 */
UCLASS()
class DELTA_API ADeltaAllyController : public ADeltaAIController
{
	GENERATED_BODY()

public:
	ADeltaAllyController();

	ADeltaBaseCharacter* GetPlayingCharacter();

private:
	TWeakObjectPtr<ADeltaPlayerController> PlayerController;

};
