// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeltaHUD.h"

#include "UI/DeltaBaseWidget.h"

void ADeltaHUD::BeginPlay()
{
	Super::BeginPlay();

	//ChangeHUDMode(EDeltaHUDMode::Main);
}

void ADeltaHUD::ChangeHUDMode(const EDeltaHUDMode HUDMode)
{
	if (CurrentWidget.Value)
	{
		for (auto CachedWidget : DeltaWidgets)
		{
			CachedWidget.Value->RemoveFromParent();
		}
		CurrentWidget.Value = nullptr;
		DeltaWidgets.Empty();
	}

	for (auto CachedClass : ResultMenuWidgetClasses)
	{
		EDeltaHUDMode CachedHUDMode = SwitchWidgetToHUD(CachedClass.Key);
		if (CachedHUDMode == HUDMode)
		{
			UDeltaBaseWidget* CachedWidget = Cast<UDeltaBaseWidget>(CreateWidget(GetWorld(), CachedClass.Value));
			if (CachedWidget)
			{
				DeltaWidgets.Add(CachedClass.Key, CachedWidget);
				CachedWidget->AddToViewport();
				CachedWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	SetHUDInit(HUDMode);

}

void ADeltaHUD::ChangeWidgetType(const EDeltaWidgetType WidgetType, const bool bDoHiddenBefore)
{
	if (!DeltaWidgets.Contains(WidgetType)) return;

	if (bDoHiddenBefore)
	{
		CurrentWidget.Value->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrentWidget.Key = WidgetType;
	CurrentWidget.Value = DeltaWidgets[WidgetType];
	CurrentWidget.Value->SetVisibility(ESlateVisibility::Visible);

}

void ADeltaHUD::SetHUDInit(const EDeltaHUDMode HUDMode)
{
	if (HUDMode == EDeltaHUDMode::Main)
	{
		if (!DeltaWidgets.Find(EDeltaWidgetType::Main_Main))
		{
			UE_LOG(LogTemp, Warning, TEXT("Main Widget need to Settings"));
			return;
		}

		CurrentWidget = TPair<EDeltaWidgetType, UDeltaBaseWidget*>(EDeltaWidgetType::Main_Main, DeltaWidgets[EDeltaWidgetType::Main_Main]);
	}
	else if (HUDMode == EDeltaHUDMode::Play)
	{
		if (!DeltaWidgets.Find(EDeltaWidgetType::Play_Play))
		{
			UE_LOG(LogTemp, Warning, TEXT("Play Widget need to Settings"));
			return;
		}

		CurrentWidget = TPair<EDeltaWidgetType, UDeltaBaseWidget*>(EDeltaWidgetType::Play_Play, DeltaWidgets[EDeltaWidgetType::Play_Play]);
	}
	
	CurrentWidget.Value->SetVisibility(ESlateVisibility::Visible);
	

}

EDeltaHUDMode ADeltaHUD::SwitchWidgetToHUD(const EDeltaWidgetType WidgetType)
{
	switch (WidgetType)
	{
	case EDeltaWidgetType::Main_Main:
	case EDeltaWidgetType::Main_Option:
		return EDeltaHUDMode::Main;
	case EDeltaWidgetType::Play_Play:
	case EDeltaWidgetType::Play_Pause:
	case EDeltaWidgetType::Play_Lose:
	case EDeltaWidgetType::Play_Win:
		return EDeltaHUDMode::Play;
	default:
		check(false);
		return EDeltaHUDMode::None;
	}
}
