// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notify/AnimNotify_PlayNiagaraFromPool.h"

#include "Components/NiagaraPoolingComponent.h"
#include "VFX/Actor/PoolingNiagaraActor.h"

void UAnimNotify_PlayNiagaraFromPool::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	if (!NiagaraActorClass) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	if (UNiagaraPoolingComponent* NiagaraPoolingComp = Owner->FindComponentByClass<UNiagaraPoolingComponent>())
	{
		NiagaraPoolingComp->PlayNiagara(NiagaraActorClass, LocationOffset, RotationOffset, Attached, SocketName);
	}

}
