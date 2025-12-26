// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraPoolingComponent.generated.h"


class APoolingNiagaraActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UNiagaraPoolingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNiagaraPoolingComponent();

	void PlayNiagara(const TSubclassOf<APoolingNiagaraActor> NiagaraActorClass, const FVector& LocationOffset, const FRotator& RotationOffset, 
		const bool bIsAttached, const FName& SocketName = NAME_None);

	/**
	 * Using when niagara looping
	 */
	void EndNiagara();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> AttachComp;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<APoolingNiagaraActor> CachedNiagaraActor = nullptr;
	
};
