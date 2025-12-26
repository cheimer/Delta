// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DeltaBaseWidget.h"
#include "SkillInfoWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DELTA_API USkillInfoWidget : public UDeltaBaseWidget
{
	GENERATED_BODY()

public:
	void SetImages(TArray<UTexture2D*>& ImageTextures);
	void SetCostTexts(TArray<int> Costs);
	void SetSelectImage(int Index, bool bIsSelect);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FVector2D ImagesSize = FVector2D(32.0f, 32.0f);

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UImage* SkillImage0;
	UPROPERTY(meta = (BindWidget))
	UImage* SkillImage1;
	UPROPERTY(meta = (BindWidget))
	UImage* SkillImage2;
	
	UPROPERTY(meta = (BindWidget))
	UImage* SkillSelectImage0;
	UPROPERTY(meta = (BindWidget))
	UImage* SkillSelectImage1;
	UPROPERTY(meta = (BindWidget))
	UImage* SkillSelectImage2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillCostText0;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillCostText1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillCostText2;

private:
	UPROPERTY()
	TArray<UImage*> ImageArray;
	UPROPERTY()
	TArray<UImage*> SelectImageArray;
	UPROPERTY()
	TArray<UTextBlock*> CostTextArray;

};
