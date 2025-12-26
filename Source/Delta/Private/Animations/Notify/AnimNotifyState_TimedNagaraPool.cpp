// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notify/AnimNotifyState_TimedNagaraPool.h"

#include "Components/NiagaraPoolingComponent.h"
#include "VFX/Actor/PoolingNiagaraActor.h"

void UAnimNotifyState_TimedNagaraPool::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;
	if (!NiagaraActorClass) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	if (UNiagaraPoolingComponent* NiagaraPoolingComp = Owner->FindComponentByClass<UNiagaraPoolingComponent>())
	{
		NiagaraPoolingComp->PlayNiagara(NiagaraActorClass, LocationOffset, RotationOffset, Attached, SocketName);
	}

}

void UAnimNotifyState_TimedNagaraPool::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	if (UNiagaraPoolingComponent* NiagaraPoolingComp = Owner->FindComponentByClass<UNiagaraPoolingComponent>())
	{
		NiagaraPoolingComp->EndNiagara();
	}

}
