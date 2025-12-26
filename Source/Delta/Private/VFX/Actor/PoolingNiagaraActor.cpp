// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/Actor/PoolingNiagaraActor.h"

#include "NiagaraComponent.h"
#include "Subsystem/ObjectPoolSubsystem.h"

void APoolingNiagaraActor::BeginPlay()
{
	Super::BeginPlay();

	SetDestroyOnSystemFinish(false);
	if (GetNiagaraComponent())
	{
		GetNiagaraComponent()->OnSystemFinished.AddUniqueDynamic(this, &APoolingNiagaraActor::OnNiagaraFinishedReturnPool);
	}
}

void APoolingNiagaraActor::OnNiagaraFinishedReturnPool(UNiagaraComponent* FinishedComponent)
{
	UObjectPoolSubsystem* ObjectPool = UObjectPoolSubsystem::Get(GetWorld());
	if (!ObjectPool) return;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ObjectPool->ReturnObject(this);
}

void APoolingNiagaraActor::PlayNiagara()
{
	if (GetNiagaraComponent())
	{
		GetNiagaraComponent()->Activate(true);
	}
}

void APoolingNiagaraActor::EndNiagara()
{
	if (GetNiagaraComponent())
	{
		GetNiagaraComponent()->DeactivateImmediate();
	}
}
