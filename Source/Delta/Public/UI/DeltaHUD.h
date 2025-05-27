// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DeltaHUD.generated.h"

UENUM(BlueprintType)
enum class EDeltaHUDMode : uint8
{
	None,
	Main,
	Play
};

UENUM(BlueprintType)
enum class EDeltaWidgetType : uint8
{
	None_None,
	
	Main_Main,
	Main_Option,

	Play_Play,
	Play_Pause,
	Play_Lose,
	Play_Win
};

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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TMap<EDeltaWidgetType, TSubclassOf<UDeltaBaseWidget>> ResultMenuWidgetClasses;

private:
	UPROPERTY()
	TMap<EDeltaWidgetType, UDeltaBaseWidget*> DeltaWidgets;

	TPair<EDeltaWidgetType, UDeltaBaseWidget*> CurrentWidget;

	EDeltaHUDMode SwitchWidgetToHUD(const EDeltaWidgetType WidgetType);
	void SetHUDInit(const EDeltaHUDMode HUDMode);
	
};
