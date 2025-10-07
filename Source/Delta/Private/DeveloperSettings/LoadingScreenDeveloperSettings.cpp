// Fill out your copyright notice in the Description page of Project Settings.


#include "DeveloperSettings/LoadingScreenDeveloperSettings.h"

#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> ULoadingScreenDeveloperSettings::GetLoadingScreenWidgetClassChecked() const
{
	check(!SoftLoadingScreenWidgetClass.IsNull());
	
	TSubclassOf<UUserWidget> LoadingScreenWidget = SoftLoadingScreenWidgetClass.LoadSynchronous();
	
	return LoadingScreenWidget;
}
