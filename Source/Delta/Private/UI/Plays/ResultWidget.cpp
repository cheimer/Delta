// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Plays/ResultWidget.h"

#include "CommonButtonBase.h"
#include "CommonNumericTextBlock.h"
#include "CommonRichTextBlock.h"
#include "Controllers/DeltaPlayerController.h"

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerController = PlayerController ? PlayerController : Cast<ADeltaPlayerController>(GetOwningPlayer());
	if (!PlayerController) return;

	check(MainButton);
	MainButton->OnClicked().AddLambda(
		[this]()
		{
			PlayerController->GoMain(false);
		});

	check(QuitButton);
	QuitButton->OnClicked().AddLambda(
		[this]()
		{
			PlayerController->QuitGame(false);
		});

}

void UResultWidget::NativeDestruct()
{
	Super::NativeDestruct();

	MainButton->OnClicked().RemoveAll(this);
	QuitButton->OnClicked().RemoveAll(this);
}

void UResultWidget::OpenWidget()
{
	Super::OpenWidget();

	PlayerController = PlayerController ? PlayerController : Cast<ADeltaPlayerController>(GetOwningPlayer());
	if (!PlayerController)
	{
		ResultCommonRichText->SetText(FText());
		TimeCommonNumericText->SetCurrentValue(0.0f);
		RemainHealthCommonNumericText->SetCurrentValue(0.0f);
		TotalDamageCommonNumericText->SetCurrentValue(0.0f);
		return;
	}

	FText ResultText = PlayerController->IsPlayerWin() ? FText::FromString("<Win>YOU WIN !</>") : FText::FromString("<Lose>YOU Lose...</>");

	ResultCommonRichText->SetText(ResultText);
	TimeCommonNumericText->SetCurrentValue(PlayerController->GetPlayingTime());
	RemainHealthCommonNumericText->SetCurrentValue(PlayerController->GetPlayerHealth());
	TotalDamageCommonNumericText->SetCurrentValue(PlayerController->GetTotalDamage());
	
}
