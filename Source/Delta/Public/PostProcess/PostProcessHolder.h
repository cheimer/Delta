// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PostProcessHolder.generated.h"

class UPostProcessComponent;
class UDamageVignetteComponent;

UCLASS()
class DELTA_API APostProcessHolder : public AActor
{
	GENERATED_BODY()
	
public:	
	APostProcessHolder();

	void RegisterMaterial(UMaterialInstanceDynamic* RegisterMI);

	void UpdateDamageVignette(const float HealthRate);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPostProcessComponent* PostProcessComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDamageVignetteComponent* DamageVignetteComponent;


};
