// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CutSceneComponent.generated.h"

class ULevelSequencePlayer;

UENUM()
enum class ECutSceneType
{
	Death
};

class ALevelSequenceActor;
class ULevelSequence;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UCutSceneComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCutSceneComponent();

	void StartCutScene(const ECutSceneType SceneType);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOwnerDeath(AActor* DeathActor);

	UFUNCTION()
	void EndCutScene();

	UPROPERTY(EditAnywhere, Category = "Death")
	ULevelSequence* DeathSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool bHideHUD = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool bDisableInput = true;

private:
	UPROPERTY()
	ALevelSequenceActor* SequenceActor;

	UPROPERTY()
	ULevelSequencePlayer* SequencePlayer;
	
};
