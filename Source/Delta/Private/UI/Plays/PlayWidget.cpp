// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Plays/PlayWidget.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "Components/Image.h"
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

	if (PlayerController->GetTeamMembers().IsValidIndex(0))
	{
		FPlayableInfo PlayableInfo0;
		PlayableInfo0.PlayableCharacter = PlayerController->GetTeamMembers()[0];
		PlayableInfo0.Icon = PlayerImage;
		PlayableInfo0.HealthBar = HealthBar;
		PlayableInfo0.ManaBar = ManaBar;
		PlayableInfos.Add(PlayableInfo0);
	}

	if (PlayerController->GetTeamMembers().IsValidIndex(1))
	{
		FPlayableInfo PlayableInfo1;
		PlayableInfo1.PlayableCharacter = PlayerController->GetTeamMembers()[1];
		PlayableInfo1.Icon = AllyImage0;
		PlayableInfo1.HealthBar = AllyHealthBar0;
		PlayableInfo1.ManaBar = AllyManaBar0;
		PlayableInfos.Add(PlayableInfo1);
	}
	if (PlayerController->GetTeamMembers().IsValidIndex(2))
	{
		FPlayableInfo PlayableInfo2;
		PlayableInfo2.PlayableCharacter = PlayerController->GetTeamMembers()[2];
		PlayableInfo2.Icon = AllyImage1;
		PlayableInfo2.HealthBar = AllyHealthBar1;
		PlayableInfo2.ManaBar = AllyManaBar1;
		PlayableInfos.Add(PlayableInfo2);
	}

	for (int i = 0; i < PlayableInfos.Num(); i++)
	{
		if (UHealthComponent* HealthComp = Cast<UHealthComponent>(PlayableInfos[i].PlayableCharacter->FindComponentByClass<UHealthComponent>()))
		{
			float CurrentHealth = HealthComp->GetCurrentHealth();
			float MaxHealth = HealthComp->GetMaxHealth();
			HandleHealthChanged(PlayableInfos[i].PlayableCharacter.Get(), CurrentHealth, MaxHealth, false);
			
			HealthComp->OnHealthChanged.AddUniqueDynamic(this, &ThisClass::HandleHealthChanged);
		}
		
		if (UManaComponent* ManaComp = Cast<UManaComponent>(PlayableInfos[i].PlayableCharacter->FindComponentByClass<UManaComponent>()))
		{
			float CurrentMana = ManaComp->GetCurrentMana();
			float MaxMana = ManaComp->GetMaxMana();
			HandleManaChanged(PlayableInfos[i].PlayableCharacter.Get(), CurrentMana, MaxMana);
			
			ManaComp->OnManaChanged.AddUniqueDynamic(this, &ThisClass::HandleManaChanged);
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

void UPlayWidget::HandleHealthChanged(AActor* ChangedActor, float CurrentHealth, float MaxHealth, bool bIsDamaged)
{
	for (auto ActorInfo : PlayableInfos)
	{
		if (ActorInfo.PlayableCharacter == ChangedActor)
		{
			ActorInfo.HealthBar->SetPercent(CurrentHealth / MaxHealth);
			break;
		}
	}

	if (ADeltaPlayableCharacter* PlayingActor = Cast<ADeltaPlayableCharacter>(ChangedActor))
	{
		if (PlayingActor->GetIsPlayerControlled())
		{
			CurrentHealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
			MaxHealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxHealth)));
		}
	}
}

void UPlayWidget::HandleManaChanged(AActor* ChangedActor, float CurrentMana, float MaxMana)
{
	for (auto ActorInfo : PlayableInfos)
	{
		if (ActorInfo.PlayableCharacter == ChangedActor)
		{
			ActorInfo.ManaBar->SetPercent(CurrentMana / MaxMana);
			break;
		}
	}

	if (ADeltaPlayableCharacter* PlayingActor = Cast<ADeltaPlayableCharacter>(ChangedActor))
	{
		if (PlayingActor->GetIsPlayerControlled())
		{
			CurrentManaText->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int>(CurrentMana))));
			MaxManaText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxMana)));
		}
	}
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

void UPlayWidget::ChangeCharacter(const AActor* CurrentCharacter, const AActor* NewCharacter)
{
	int BeforeIndex = -1, NextIndex = -1;
	for (int i = 0; i < PlayableInfos.Num(); i++)
	{
		if (!PlayableInfos[i].PlayableCharacter.IsValid()) continue;

		if (PlayableInfos[i].PlayableCharacter.Get() == CurrentCharacter)
		{
			BeforeIndex = i;
		}
		else if (PlayableInfos[i].PlayableCharacter.Get() == NewCharacter)
		{
			NextIndex = i;
		}
	}

	if (BeforeIndex == -1 || NextIndex == -1) return;

	ChangeInfo(PlayableInfos[BeforeIndex], PlayableInfos[NextIndex]);
}

void UPlayWidget::ChangeInfo(FPlayableInfo& BeforeInfo, FPlayableInfo& AfterInfo)
{
	AActor* BeforeCharacter = BeforeInfo.PlayableCharacter.Get();
	BeforeInfo.PlayableCharacter = AfterInfo.PlayableCharacter;
	AfterInfo.PlayableCharacter = BeforeCharacter;

	FSlateBrush BeforeIcon = BeforeInfo.Icon->GetBrush();
	BeforeInfo.Icon->SetBrush(AfterInfo.Icon->GetBrush());
	AfterInfo.Icon->SetBrush(BeforeIcon);
}
