// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "Engine/DataAsset.h"
#include "UIDataAsset.generated.h"


class UDeltaBaseWidget;

USTRUCT(BlueprintType)
struct FUIWidgetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDeltaHUDMode HUDMode = EDeltaHUDMode::None;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDeltaWidgetType WidgetType = EDeltaWidgetType::None;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDeltaBaseWidget> WidgetClass;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDefaultWidget = false;
	
};

/**
 * 
 */
UCLASS()
class DELTA_API UUIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FUIWidgetInfo> UIWidgetInfos;
	
};
