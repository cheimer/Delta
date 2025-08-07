// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DeltaBaseWidget.h"
#include "SkillInfoWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class DELTA_API USkillInfoWidget : public UDeltaBaseWidget
{
	GENERATED_BODY()

public:
	void SetImages(TArray<const UTexture2D*>& ImageTextures);
	
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

private:
	UPROPERTY()
	TArray<UImage*> ImageArray;

};
