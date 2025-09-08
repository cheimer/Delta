// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeltaHUD.h"

#include "UI/DeltaBaseWidget.h"
#include "UI/PlayWidget.h"

void ADeltaHUD::BeginPlay()
{
	Super::BeginPlay();

	ChangeHUDMode(EDeltaHUDMode::Play);
}

void ADeltaHUD::ChangeHUDMode(const EDeltaHUDMode HUDMode)
{
	if (CurrentWidget)
	{
		for (auto CachedWidget : DeltaWidgets)
		{
			CachedWidget.Value->RemoveFromParent();
			CachedWidget.Value->MarkAsGarbage();
		}
		CurrentWidget = nullptr;
		DeltaWidgets.Empty();
	}

	if (!IsValid(UIDataAsset)) return;

	int DefaultWidgetNum = 0;
	for (auto WidgetInfo : UIDataAsset->UIWidgetInfos)
	{
		if (WidgetInfo.HUDMode == HUDMode)
		{
			if (UDeltaBaseWidget* CachedWidget = Cast<UDeltaBaseWidget>(CreateWidget(GetWorld(), WidgetInfo.WidgetClass)))
			{
				DeltaWidgets.Add(WidgetInfo.WidgetType, CachedWidget);
				
				CachedWidget->InItWidget(WidgetInfo.WidgetType);
				CachedWidget->AddToViewport();
				if (WidgetInfo.bIsDefaultWidget)
				{
					CachedWidget->SetVisibility(ESlateVisibility::Visible);
					DefaultWidgetNum++;
					CurrentWidget = CachedWidget;
				}
				else
				{
					CachedWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
	
	check(DefaultWidgetNum == 1);

}

void ADeltaHUD::ChangeWidgetType(const EDeltaWidgetType WidgetType, const bool bDoHiddenBefore)
{
	if (!DeltaWidgets.Contains(WidgetType)) return;

	if (bDoHiddenBefore && IsValid(CurrentWidget))
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	CurrentWidget = DeltaWidgets[WidgetType];
	CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	
}

void ADeltaHUD::ShowTarget(const AActor* Target) const
{
	if (!DeltaWidgets.Contains(EDeltaWidgetType::Play)) return;

	if (UPlayWidget* PlayWidget = Cast<UPlayWidget>(DeltaWidgets[EDeltaWidgetType::Play]))
	{
		PlayWidget->ShowTargetInfo(Target);
	}
}

void ADeltaHUD::HideTarget() const
{
	if (!DeltaWidgets.Contains(EDeltaWidgetType::Play)) return;

	if (UPlayWidget* PlayWidget = Cast<UPlayWidget>(DeltaWidgets[EDeltaWidgetType::Play]))
	{
		PlayWidget->HideTargetInfo();
	}
}

void ADeltaHUD::ChangeSkillSet(int BeforeIndex, int AfterIndex)
{
	if (!DeltaWidgets.Contains(EDeltaWidgetType::Play)) return;

	if (UPlayWidget* PlayWidget = Cast<UPlayWidget>(DeltaWidgets[EDeltaWidgetType::Play]))
	{
		PlayWidget->ChangeSkillSet(BeforeIndex, AfterIndex);
	}
}

void ADeltaHUD::SelectSkill(int SelectSet, int SelectIndex, bool bIsSelect)
{
	if (!DeltaWidgets.Contains(EDeltaWidgetType::Play)) return;

	if (UPlayWidget* PlayWidget = Cast<UPlayWidget>(DeltaWidgets[EDeltaWidgetType::Play]))
	{
		PlayWidget->SelectSkill(SelectSet, SelectIndex, bIsSelect);
	}

}

void ADeltaHUD::SetHudVisibility(const ESlateVisibility Visibility)
{
	CurrentWidget->SetVisibility(Visibility);
}
