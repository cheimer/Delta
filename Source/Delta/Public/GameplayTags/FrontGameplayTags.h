// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace FrontGameplayTags
{
	//Front Widget Stack
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_WidgetStack_Modal);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_WidgetStack_GameMenu);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_WidgetStack_GameHud);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_WidgetStack_Front);
	
	//Front Widgets
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_Widget_PressAnyKeyScreen);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_Widget_MainMenuScreen);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_Widget_OptionsScreen);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_Widget_ConfirmScreen);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_Widget_KeyRemapScreen);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_Widget_StoryScreen);
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_Widget_CreditScreen);

	//Front Options Image
	DELTA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Front_Image_TestImage);
	
}