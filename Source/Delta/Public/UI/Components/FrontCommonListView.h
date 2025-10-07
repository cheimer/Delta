// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontCommonListView.generated.h"

class UDataListEntryMappingDataAsset;
/**
 * 
 */
UCLASS()
class DELTA_API UFrontCommonListView : public UCommonListView
{
	GENERATED_BODY()

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;

private:
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	UDataListEntryMappingDataAsset* DataListEntryMapping;
	
};
