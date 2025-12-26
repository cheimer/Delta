// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"
#include "PoolingNiagaraActor.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API APoolingNiagaraActor : public ANiagaraActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void PlayNiagara();
	void EndNiagara();

private:
	UFUNCTION()
	void OnNiagaraFinishedReturnPool(UNiagaraComponent* FinishedComponent);

};
