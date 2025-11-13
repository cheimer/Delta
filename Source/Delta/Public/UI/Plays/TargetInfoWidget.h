// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DeltaBaseWidget.h"
#include "TargetInfoWidget.generated.h"

class UHealthComponent;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DELTA_API UTargetInfoWidget : public UDeltaBaseWidget
{
	GENERATED_BODY()
	
public:
	void RegisterInfo(const AActor* Target);
	void RemoveInfo();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleHealthChanged(AActor* ChangedActor, float CurrentHealth, float MaxHealth, bool bIsDamaged);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TWeakObjectPtr<UHealthComponent> TargetHealth;

private:
	TWeakObjectPtr<AActor> TargetActor;
	
};
