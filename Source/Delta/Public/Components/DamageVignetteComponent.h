// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageVignetteComponent.generated.h"


class UPostProcessComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTA_API UDamageVignetteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDamageVignetteComponent();

	void UpdateDamageVignette(const float HealthRate);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	UMaterialInstance* DamageVignetteMI;

	UPROPERTY(EditAnywhere, Category = "Value")
	FName InnerVignette = TEXT("InnerVignette");

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DamageVignetteMaterialDynamic;
	
};
