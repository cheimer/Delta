// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/UI/UIDataAsset.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "GameFramework/HUD.h"
#include "DeltaHUD.generated.h"

class UDeltaBaseWidget;
/**
 * 
 */
UCLASS()
class DELTA_API ADeltaHUD : public AHUD
{
	GENERATED_BODY() 

public:
	void ChangeHUDMode(const EDeltaHUDMode HUDMode);
	void ChangeWidgetType(const EDeltaWidgetType WidgetType, bool bDoHiddenBefore = true);

	void ShowTarget(const AActor* Target) const;
	void HideTarget() const;
	
	void ChangeSkillSet(int BeforeIndex, int AfterIndex);
	void SelectSkill(int SelectSet, int SelectIndex, bool bIsSelect);

	void SetHudVisibility(const ESlateVisibility Visibility);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI Asset")
	UUIDataAsset* UIDataAsset;

private:
	UPROPERTY()
	TMap<EDeltaWidgetType, UDeltaBaseWidget*> DeltaWidgets;
	UPROPERTY()
	UDeltaBaseWidget* CurrentWidget;

};
