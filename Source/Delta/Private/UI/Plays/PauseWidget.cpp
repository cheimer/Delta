// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Plays/PauseWidget.h"

#include "CommonButtonBase.h"
#include "CommonNumericTextBlock.h"
#include "Controllers/DeltaPlayerController.h"
#include "GameModes/DeltaBaseGameMode.h"
#include "Helper/DeltaDebugHelper.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerController = PlayerController ? PlayerController : Cast<ADeltaPlayerController>(GetOwningPlayer());
	if (!PlayerController) return;

	check(ContinueButton);
	ContinueButton->OnClicked().AddLambda(
		[this]()
		{
			PlayerController->ContinueGame();
		});
	check(MainButton);
	MainButton->OnClicked().AddLambda(
		[this]()
		{
			PlayerController->GoMain();
		});

	check(QuitButton);
	QuitButton->OnClicked().AddLambda(
		[this]()
		{
			PlayerController->QuitGame();
		});

}

void UPauseWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	ContinueButton->OnClicked().RemoveAll(this);
	MainButton->OnClicked().RemoveAll(this);
	QuitButton->OnClicked().RemoveAll(this);
}

void UPauseWidget::OpenWidget()
{
	Super::OpenWidget();

	PlayerController = PlayerController ? PlayerController : Cast<ADeltaPlayerController>(GetOwningPlayer());
	if (!PlayerController)
	{
		TimeCommonNumericText->SetCurrentValue(0.0f);
		return;
	}

	TimeCommonNumericText->SetCurrentValue(PlayerController->GetPlayingTime());
	
}
