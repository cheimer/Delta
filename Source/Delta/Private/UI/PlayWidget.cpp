// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayWidget.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/ProgressBar.h"
#include "Controllers/DeltaPlayerController.h"
#include "UI/SkillInfoWidget.h"
#include "UI/TargetInfoWidget.h"

void UPlayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UPlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	const APawn* OwnerPawn = GetOwningPlayer() ? Cast<APawn>(GetOwningPlayer()->GetPawn()) : nullptr;
	if (!OwnerPawn) return;

	if (HealthBar)
	{
		HealthBar->SetPercent(1.0f);
		{
			if (UHealthComponent* HealthComp = Cast<UHealthComponent>(OwnerPawn->FindComponentByClass<UHealthComponent>()))
			{
				HealthComp->OnHealthChanged.AddDynamic(this, &ThisClass::HandleHealthChanged);
			}
		}
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
		if (ADeltaPlayerController* DeltaPC = Cast<ADeltaPlayerController>(GetOwningPlayer()))
		{
			for (int i = 0; i < SkillInfoArray.Num(); i++)
			{
				SkillInfoArray[i]->SetImages(DeltaPC->GetSkillTextures(i));
			}
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

void UPlayWidget::HandleHealthChanged(float CurrentHealth, float MaxHealth, bool bIsDamaged)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
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
		UE_LOG(LogTemp, Warning, TEXT("Q Left"));
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
		UE_LOG(LogTemp, Warning, TEXT("E Right"));
		if (SkillChangeAnims.Contains(BeforeIndex) && SkillChangeAnims.Contains(AfterIndex))
		{
			if (SkillChangeAnims[BeforeIndex].RightAnim)
			{
				PlayAnimation(SkillChangeAnims[BeforeIndex].RightAnim);
			}
		}
	}
}
