// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/NiagaraPoolingComponent.h"

#include "GameFramework/Character.h"
#include "Subsystem/ObjectPoolSubsystem.h"
#include "VFX/Actor/PoolingNiagaraActor.h"

UNiagaraPoolingComponent::UNiagaraPoolingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UNiagaraPoolingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()) return;

	if (!AttachComp)
	{
		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			AttachComp = Character->GetMesh();
		}
		else if (USkeletalMeshComponent* SkeletalMeshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
		{
			AttachComp = SkeletalMeshComp;
		}
		else if (UStaticMeshComponent* StaticMeshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
		{
			AttachComp = StaticMeshComp;
		}
		else
		{
			AttachComp = GetOwner()->GetRootComponent();
		}
	}

}

void UNiagaraPoolingComponent::PlayNiagara(const TSubclassOf<APoolingNiagaraActor> NiagaraActorClass, const FVector& LocationOffset, const FRotator& RotationOffset, const bool bIsAttached, const FName& SocketName)
{
	if (!AttachComp) return;
	
	UObjectPoolSubsystem* ObjectPool = UObjectPoolSubsystem::Get(this);
	if (!ObjectPool) return;

	APoolingNiagaraActor* NiagaraObject = Cast<APoolingNiagaraActor>(ObjectPool->GetObject(NiagaraActorClass));
	if (!NiagaraObject) return;

	if (bIsAttached)
	{
		NiagaraObject->AttachToComponent(AttachComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		NiagaraObject->AddActorLocalOffset(LocationOffset);
		NiagaraObject->AddActorLocalRotation(RotationOffset);
	}
	else
	{
		NiagaraObject->SetActorLocation(AttachComp->GetComponentLocation() + LocationOffset);
		NiagaraObject->SetActorRotation(AttachComp->GetComponentRotation() + RotationOffset);
	}
	
	NiagaraObject->PlayNiagara();
	
	CachedNiagaraActor = NiagaraObject;
}

void UNiagaraPoolingComponent::EndNiagara()
{
	if (CachedNiagaraActor.IsValid())
	{
		CachedNiagaraActor->EndNiagara();
	}
}
