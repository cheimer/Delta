// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Plays/TargetInfoWidget.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UTargetInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NameText->SetVisibility(ESlateVisibility::Hidden);
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
}

void UTargetInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveInfo();
}

void UTargetInfoWidget::RegisterInfo(const AActor* Target)
{
	if (!IsValid(Target)) return;
	if (TargetActor.IsValid() && TargetActor == Target) return;
	
	if (const ADeltaBaseCharacter* DeltaTarget = Cast<ADeltaBaseCharacter>(Target))
	{
		NameText->SetText(FText::FromString(DeltaTarget->GetDisplayName()));
		NameText->SetVisibility(ESlateVisibility::Visible);
	}

	TargetHealth = Target->FindComponentByClass<UHealthComponent>();
	if (TargetHealth.IsValid())
	{
		TargetHealth->OnHealthChanged.AddUniqueDynamic(this, &ThisClass::HandleHealthChanged);
		HealthBar->SetPercent(TargetHealth->GetHealthPercentage());
		HealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void UTargetInfoWidget::RemoveInfo()
{
	if (TargetHealth.IsValid() && TargetHealth->OnHealthChanged.IsAlreadyBound(this, &ThisClass::HandleHealthChanged))
	{
		TargetHealth->OnHealthChanged.RemoveDynamic(this, &ThisClass::HandleHealthChanged);
	}

	NameText->SetVisibility(ESlateVisibility::Hidden);
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
}

void UTargetInfoWidget::HandleHealthChanged(AActor* ChangedActor, float CurrentHealth, float MaxHealth, bool bIsDamaged)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
}
