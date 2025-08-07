// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillInfoWidget.h"

#include "Components/Image.h"

void USkillInfoWidget::SetImages(TArray<const UTexture2D*>& ImageTextures)
{
	if (ImageTextures.Num() != ImageArray.Num()) return;
	
	for (int i = 0; i < ImageTextures.Num(); i++)
	{
		if (IsValid(ImageTextures[i]) && IsValid(ImageArray[i]))
		{
			ImageArray[i]->SetBrushFromSoftTexture(ImageTextures[i]);
		}
	}
	
}

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
	
}
