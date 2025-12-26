// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTags/FrontGameplayTags.h"

namespace FrontGameplayTags
{
	//Front Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(Front_WidgetStack_Modal, "Front.WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(Front_WdigetStack_GameMenu, "Front.WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(Front_WdigetStack_GameHud, "Front.WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(Front_WdigetStack_Front, "Front.WidgetStack.Front");
	
	//Front Widgets
	UE_DEFINE_GAMEPLAY_TAG(Front_Widget_PressAnyKeyScreen, "Front.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Front_Widget_MainMenuScreen, "Front.Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(Front_Widget_OptionsScreen, "Front.Widget.OptionsScreen");
	UE_DEFINE_GAMEPLAY_TAG(Front_Widget_ConfirmScreen, "Front.Widget.ConfirmScreen");
	UE_DEFINE_GAMEPLAY_TAG(Front_Widget_KeyRemapScreen, "Front.Widget.KeyRemapScreen");
	UE_DEFINE_GAMEPLAY_TAG(Front_Widget_StoryScreen, "Front.Widget.StoryScreen");
	UE_DEFINE_GAMEPLAY_TAG(Front_Widget_CreditScreen, "Front.Widget.CreditScreen");
	
	//Front Options Image
	UE_DEFINE_GAMEPLAY_TAG(Front_Image_TestImage, "Front.Image.TestImage");
	
}