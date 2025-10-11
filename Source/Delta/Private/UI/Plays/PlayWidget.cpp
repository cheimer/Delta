// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Plays/PlayWidget.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/ManaComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Controllers/DeltaPlayerController.h"
#include "UI/Plays/SkillInfoWidget.h"
#include "UI/Plays/TargetInfoWidget.h"

void UPlayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UPlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	const APawn* OwnerPawn = GetOwningPlayer() ? Cast<APawn>(GetOwningPlayer()->GetPawn()) : nullptr;
	if (!OwnerPawn) return;

	PlayerController = PlayerController ? PlayerController : Cast<ADeltaPlayerController>(GetOwningPlayer());
	if (!PlayerController) return;

	if (HealthBar && CurrentHealthText && MaxHealthText)
	{
		if (UHealthComponent* HealthComp = Cast<UHealthComponent>(OwnerPawn->FindComponentByClass<UHealthComponent>()))
		{
			float CurrentHealth =  HealthComp->GetCurrentHealth();
			float MaxHealth = HealthComp->GetMaxHealth();
			HandleHealthChanged(CurrentHealth, MaxHealth, false);
			
			HealthComp->OnHealthChanged.AddUniqueDynamic(this, &ThisClass::HandleHealthChanged);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Health related widget not construct"));
	}
	
	if (ManaBar && CurrentManaText && MaxManaText)
	{
		if (UManaComponent* ManaComp = Cast<UManaComponent>(OwnerPawn->FindComponentByClass<UManaComponent>()))
		{
			float CurrentMana = ManaComp->GetCurrentMana();
			float MaxMana = ManaComp->GetMaxMana();
			HandleManaChanged(CurrentMana, MaxMana);
			
			ManaComp->OnManaChanged.AddUniqueDynamic(this, &ThisClass::HandleManaChanged);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Health related widget not construct"));
	}

	if (TargetInfoWidget)
	{
		TargetInfoWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	SkillInfoArray.Add(SkillInfoWidget0);
	SkillInfoArray.Add(SkillInfoWidget1);
	SkillInfoArray.Add(SkillInfoWidget2);

	if (!SkillInfoArray.IsEmpty())
	{
		for (int i = 0; i < SkillInfoArray.Num(); i++)
		{
			SkillInfoArray[i]->SetImages(PlayerController->GetSkillTextures(i));
			SkillInfoArray[i]->SetCostTexts(PlayerController->GetSkillCosts(i));
		}
	}
	
	FChangeSkillSet CachedChangeSkillSet0;
	CachedChangeSkillSet0.CurrentIndex = 0;
	CachedChangeSkillSet0.RightAnim = SkillChange0To1Anim;
	SkillChangeAnims.Add(0, CachedChangeSkillSet0);
	
	FChangeSkillSet CachedChangeSkillSet1;
	CachedChangeSkillSet1.CurrentIndex = 1;
	CachedChangeSkillSet1.LeftAnim = SkillChange1To0Anim;
	CachedChangeSkillSet1.RightAnim = SkillChange1To2Anim;
	SkillChangeAnims.Add(1, CachedChangeSkillSet1);

	FChangeSkillSet CachedChangeSkillSet2;
	CachedChangeSkillSet2.CurrentIndex = 2;
	CachedChangeSkillSet2.LeftAnim = SkillChange2To1Anim;
	SkillChangeAnims.Add(2, CachedChangeSkillSet2);
	
}

void UPlayWidget::NativeDestruct()
{
	if (const APawn* OwnerPawn = GetOwningPlayer() ? Cast<APawn>(GetOwningPlayer()->GetPawn()) : nullptr)
	{
		if (UHealthComponent* HealthComp = Cast<UHealthComponent>(OwnerPawn->FindComponentByClass<UHealthComponent>()))
		{
			if (HealthComp->OnHealthChanged.IsAlreadyBound(this, &ThisClass::HandleHealthChanged))
			{
				HealthComp->OnHealthChanged.RemoveDynamic(this, &ThisClass::HandleHealthChanged);
			}
		}
		if (UManaComponent* ManaComp = Cast<UManaComponent>(OwnerPawn->FindComponentByClass<UManaComponent>()))
		{
			if (ManaComp->OnManaChanged.IsAlreadyBound(this, &ThisClass::HandleManaChanged))
			{
				ManaComp->OnManaChanged.RemoveDynamic(this, &ThisClass::HandleManaChanged);
			}
		}
	}

	Super::NativeDestruct();
}

void UPlayWidget::HandleHealthChanged(float CurrentHealth, float MaxHealth, bool bIsDamaged)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);

	CurrentHealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
	MaxHealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxHealth)));
}

void UPlayWidget::HandleManaChanged(float CurrentMana, float MaxMana)
{
	ManaBar->SetPercent(CurrentMana / MaxMana);
	
	CurrentManaText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentMana)));
	MaxManaText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxMana)));
}

void UPlayWidget::ShowTargetInfo(const AActor* Target) const
{
	TargetInfoWidget->SetVisibility(ESlateVisibility::Visible);
	TargetInfoWidget->RegisterInfo(Target);
}

void UPlayWidget::HideTargetInfo() const
{
	TargetInfoWidget->SetVisibility(ESlateVisibility::Hidden);
	TargetInfoWidget->RemoveInfo();
}

void UPlayWidget::ChangeSkillSet(const int BeforeIndex, const int AfterIndex)
{
	if (!SkillInfoArray.IsValidIndex(BeforeIndex) || !SkillInfoArray.IsValidIndex(AfterIndex)) return;

	if (BeforeIndex > AfterIndex)
	{
		if (SkillChangeAnims.Contains(BeforeIndex) && SkillChangeAnims.Contains(AfterIndex))
		{
			if (SkillChangeAnims[BeforeIndex].LeftAnim)
			{
				PlayAnimation(SkillChangeAnims[BeforeIndex].LeftAnim);
			}
		}
	}
	else
	{
		if (SkillChangeAnims.Contains(BeforeIndex) && SkillChangeAnims.Contains(AfterIndex))
		{
			if (SkillChangeAnims[BeforeIndex].RightAnim)
			{
				PlayAnimation(SkillChangeAnims[BeforeIndex].RightAnim);
			}
		}
	}
}

void UPlayWidget::SelectSkill(const int SelectSet, const int SelectIndex, const bool bIsSelect)
{
	if (!SkillInfoArray.IsValidIndex(SelectSet)) return;

	SkillInfoArray[SelectSet]->SetSelectImage(SelectIndex, bIsSelect);
}
