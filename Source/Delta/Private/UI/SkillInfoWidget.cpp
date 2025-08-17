// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillInfoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillInfoWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	ImageArray.Add(SkillImage0);
	ImageArray.Add(SkillImage1);
	ImageArray.Add(SkillImage2);
	for (int i = 0; i < ImageArray.Num(); i++)
	{
		if (ImageArray.IsValidIndex(i))
		{
			ImageArray[i]->SetDesiredSizeOverride(ImagesSize);
		}
	}

	SelectImageArray.Add(SkillSelectImage0);
	SelectImageArray.Add(SkillSelectImage1);
	SelectImageArray.Add(SkillSelectImage2);
	for (int i = 0; i < SelectImageArray.Num(); i++)
	{
		if (SelectImageArray.IsValidIndex(i))
		{
			SelectImageArray[i]->SetDesiredSizeOverride(ImagesSize);
			SelectImageArray[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	CostTextArray.Add(SkillCostText0);
	CostTextArray.Add(SkillCostText1);
	CostTextArray.Add(SkillCostText2);
	
}

void USkillInfoWidget::SetImages(TArray<UTexture2D*>& ImageTextures)
{
	if (ImageTextures.Num() != ImageArray.Num()) return;
	
	for (int i = 0; i < ImageTextures.Num(); i++)
	{
		if (IsValid(ImageTextures[i]) && IsValid(ImageArray[i]))
		{
			ImageArray[i]->SetBrushFromTexture(ImageTextures[i]);
		}
	}
	
}

void USkillInfoWidget::SetCostTexts(TArray<int> Costs)
{
	if (CostTextArray.Num() != Costs.Num()) return;

	for (int i = 0; i < CostTextArray.Num(); i++)
	{
		CostTextArray[i]->SetText(FText::FromString(FString::FromInt(Costs[i])));
	}
}

void USkillInfoWidget::SetSelectImage(int Index, bool bIsSelect)
{
	if (!SelectImageArray.IsValidIndex(Index)) return;

	ESlateVisibility SelectVisibility = bIsSelect ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	SelectImageArray[Index]->SetVisibility(SelectVisibility);
}
